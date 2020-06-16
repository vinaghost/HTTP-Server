#include <iostream>
#include <fstream>
#include <vector>

#include "server.h"
#include "throw_exception.hpp"


Server::Server() :
    result(NULL),
    sck_server(INVALID_SOCKET),
    sck_client(INVALID_SOCKET)
{
    startWSA();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getAddressinfo();
    createSocket();   
    bindSocket();
    listenSocket();
}
Server::~Server() {
    stop();
}
void Server::run() {   
    SOCKET client;

    while(true) {
        printf("\n================================================");
        printf("\n============== ... Waiting ... =================");
        printf("\n================================================\n\n");

        client = acceptSocket();

        auto s = receiveSocket(client);
        
        std::cout << "Client sent: \n";
        std::cout << s << "\n";


        if (!writeStrToClient(client, "HTTP/1.1 200 OK\r\n")){
            closesocket(client);
            continue;
        }

        auto msg = getContentFile("index.html");
        
        
        char clen[40];
        sprintf(clen, "Content-length: %ld\r\n", msg.length());
        if (!writeStrToClient(client, clen)){
            closesocket(client);
            continue;
        }

        if (!writeStrToClient(client, "Content-Type: text/html\r\n")){
            closesocket(client);
            continue;
        }

        if (!writeDataToClient(client, msg.c_str(), msg.length())){
            closesocket(client);
            continue;
        }

        printf("Sent message to client\n");
        closesocket(client);
    }
}
void Server::stop() {
    if( sck_client != INVALID_SOCKET ) {
        closesocket(sck_server);
    }

    if( sck_client != INVALID_SOCKET ) {
        closesocket(sck_client);
    }
    freeaddrinfo(result);
    WSACleanup();
}

void Server::startWSA() {
    auto iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    try {
        if ( iResult != 0 ) {
            throw SocketError("WSAStartup", WSAGetLastError());
        }
    }
    catch (SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

void Server::getAddressinfo() {
    auto iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    try {
        if ( iResult != 0 ) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            
        }
    }
    catch (SocketError &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

void Server::createSocket() {
    sck_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    try {
        if ( sck_server == INVALID_SOCKET ) {
            throw SocketCreate(WSAGetLastError());
        }
    }
    catch (SocketCreate &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

void Server::bindSocket() {    
    auto iResult = bind( sck_server, result->ai_addr, (int)result->ai_addrlen);
    
    try {
        if ( iResult == SOCKET_ERROR ) {
            throw SocketBind(WSAGetLastError());
        }
    }
    catch (SocketBind &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

void Server::listenSocket() {    
    auto iResult = listen(sck_server, SOMAXCONN);

    try {
        if ( iResult == SOCKET_ERROR ) {
            throw SocketListen(WSAGetLastError());
        }
    }
    catch (SocketListen &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

SOCKET Server::acceptSocket() {
    auto client = accept(sck_server, NULL, NULL);

    try {
        if (client == INVALID_SOCKET) {
            throw SocketAccept(WSAGetLastError());
        }
    }
    catch (SocketAccept &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
    return client;
}

std::string Server::receiveSocket(SOCKET client) {
    char recvbuf[DEFAULT_BUFLEN];

    auto iResult = recv(client, recvbuf, DEFAULT_BUFLEN, 0);
    try {
        if (iResult > 0) {
            std::cout << "Bytes received: " << iResult << "\n";
        }
        else if (iResult == 0) {
            std::cout << "Connection closing...\n";
        }
        else {
            throw SocketReceive(WSAGetLastError());
        }
    }
    catch (SocketReceive &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    } 

    return std::string(recvbuf);
}

void Server::sendSocket(SOCKET client, const std::string &data) {
    auto iResult = send( client, data.c_str(), data.length(), 0 );
    try {
        if (iResult == SOCKET_ERROR) {
            throw SocketSend(WSAGetLastError());
        }
    }
    catch (SocketSend &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }

    std::cout << "Bytes sent: " << iResult << "\n";
}

void Server::shutdownSocket(SOCKET client) {
    auto iResult = shutdown(client, SD_BOTH);
    try {
        if ( iResult == SOCKET_ERROR ) {
            throw SocketShutdown(WSAGetLastError());
        }
    }
    catch (SocketShutdown &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

std::string Server::getContentFile(const std::string &filename) {
    return std::string(readFileBinary("../html/" + filename));
}

bool Server::writeDataToClient(SOCKET socket, const char *data, int datalen) {
    const char *pData = data;

    while (datalen > 0){
        int numSent = send(socket, pData, datalen, 0);
        if (numSent <= 0){
            if (numSent == 0){
                printf("The client was not written to: disconnected\n");
            } else {
                perror("The client was not written to");
            }
            return false;
        }
        pData += numSent;
        datalen -= numSent;
    }
    return true;
}

bool Server::writeStrToClient(SOCKET socket, const std::string &str) {
    return writeDataToClient(socket, str.c_str(), str.length());
}

std::string Server::readFileBinary(const std::string &fileName) {
    std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return std::string(bytes.data(), fileSize);
}