#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include "server.h"
#include "throw_exception.hpp"
#include "request.h"
#include "response.h"

Server::Server() :
    port("80"),
    result(NULL),
    server_sck(INVALID_SOCKET),
    stop_server(false)
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
Server::Server(PCSTR port) :
    port(port),
    result(NULL),
    server_sck(INVALID_SOCKET),
    stop_server(false)
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
    server_thread = std::thread(&Server::handleConnection, this);
    server_thread.detach();

    std::string command;
    do {
        std::cout << "Type quit to stop server\n";
        std::cin >> command;
    }
    while ( command.compare("quit") != 0);
}
void Server::stop() {
    stop_server = true;
    if( server_sck != INVALID_SOCKET ) {
        closesocket(server_sck);
    }

    while(!client_sck.empty()) {
        if( client_sck.back() != INVALID_SOCKET) {
            shutdownSocket(client_sck.back());
            closesocket(client_sck.back());
        }
        client_sck.pop_back();
    }

    freeaddrinfo(result);
    WSACleanup();

    std::cout << "Server stopped";
}

void Server::handleConnection() {
    std::cout << "Server started\n"; 
    while(!stop_server) {
        
        client_sck.push_back(acceptSocket());
        
        std::thread t(&Server::handleClient, this, client_sck.size() - 1);
        t.detach();
    }
}

void Server::handleClient(size_t id) {
    auto client  = client_sck[id];

    std::lock_guard<std::mutex> guard(client_mutex);
    auto s = receiveSocket(client);
    Request req(s);
    req.showData();
    
    Response resp;
    std::string msg;

    if(req.isGetMethod()) {
        auto file = req.getHeader().at("Request Url");
        if ( file == "") {
            resp.set(301, "index.html");
        }
        else {
            msg = getContentFile(file);
            resp.set(200, msg.length());
        }

        
    }
    else if (req.isPostMethod()) {
        const auto data = req.getData();
        if( data.at("username").compare("admin") == 0 && data.at("password").compare("admin") == 0) {
            resp.set(301, "info.html");
        }
        else {
            msg = getContentFile("404.html");
            resp.set(404, msg.length());
        }
    }

    resp.showData();

    if (!writeStrToClient(client, resp.getResponse())) {
        client_sck.erase(client_sck.begin() + id);
        closesocket(client);
        return;
    }
    if( msg != "") {
        if (!writeStrToClient(client, msg)) {
            client_sck.erase(client_sck.begin() + id);
            closesocket(client);
            return;
        }
    }
    
    std::cout << "Sent message to client\n";
    client_sck.erase(client_sck.begin() + id);
    closesocket(client);
    return;
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
        abort();
    }
}

void Server::getAddressinfo() {
    auto iResult = getaddrinfo(NULL, port, &hints, &result);
    try {
        if ( iResult != 0 ) {
            throw GetAddressinfo(WSAGetLastError());            
        }
    }
    catch (GetAddressinfo &e) {
        std::cout << e.what() << "\n";
        stop();
        abort();
    }
}

void Server::createSocket() {
    server_sck = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    try {
        if ( server_sck == INVALID_SOCKET ) {
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
    auto iResult = bind( server_sck, result->ai_addr, (int)result->ai_addrlen);
    
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
    auto iResult = listen(server_sck, SOMAXCONN);

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
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    auto client = accept(server_sck, (sockaddr*)&addr, &addrlen);

    char *ip = inet_ntoa(addr.sin_addr);
    std::cout << "Accepted connection from :  " << ip << ":" << addr.sin_port <<"\n";

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
    return readFileBinary("../html/" + filename);
}

bool Server::writeDataToClient(SOCKET socket, const char *data, int datalen) {
    const char *pData = data;
    
    while (datalen > 0){
        int numSent = send(socket, pData, datalen, 0);
        if (numSent <= 0){
            if (numSent == 0){
                std::cout << "The client was not written to: disconnected\n";
            } else {
                std::cout << "The client was not written to";
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