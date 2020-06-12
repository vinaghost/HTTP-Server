#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>

#define PORT 8080

bool writeDataToClient(SOCKET socket, const char *data, int datalen) {
    const char *pData = data;

    printf("Sent: %s\n", pData);
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

bool writeStrToClient(SOCKET socket, const char *str) {
    printf("] Sent: %s\n", str);

    return writeDataToClient(socket, str, strlen(str));
}

int main(){    
    /*auto *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    int hello_len =  strlen(hello);*/

    


    // Initialize Winsock
    WSADATA wsaData;
    auto iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    /*
    [af] AF_INET = The Internet Protocol version 4 (IPv4) address family.
    
    [type] SOCK_STREAM = A socket type that provides sequenced, reliable, two-way, connection-based byte streams
    with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) 
    for the Internet address family (AF_INET or AF_INET6).
    
    [protocol] IPPROTO_TCP =  The Transmission Control Protocol (TCP). This is a possible value when the af 
    parameter is AF_INET or AF_INET6 and the type parameter is SOCK_STREAM.
    */
    auto server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if ( server == INVALID_SOCKET ) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( PORT );
    memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

    int addrlen = sizeof(addr);
    
    if (bind(server, (sockaddr *)&addr, addrlen ) == SOCKET_ERROR) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(server, 10) == SOCKET_ERROR) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    SOCKET client;

    long fsize;
    FILE *fp = fopen("../html/index.html", "rb");
    if (!fp){
        perror("The file was not opened");    
        exit(1);    
    }

    printf("The file was opened\n");

    if (fseek(fp, 0, SEEK_END) == -1){
        perror("The file was not seeked");
        exit(1);
    }

    fsize = ftell(fp);
    if (fsize == -1) {
        perror("The file size was not retrieved");
        exit(1);
    }
    rewind(fp);

    char *msg = (char*) malloc(fsize);
    if (!msg){
        perror("The file buffer was not allocated\n");
        exit(1);
    }

    if (fread(msg, fsize, 1, fp) != 1){
        perror("The file was not read\n");
        exit(1);
    }
    fclose(fp);

    printf("The file size is %ld\n", fsize);

    while(true) {
        printf("\n================================================");
        printf("\n============== ... Waiting ... =================");
        printf("\n================================================\n\n");
        if ((client = accept(server, (sockaddr *)&addr, &addrlen)) == INVALID_SOCKET) {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }
        
        char buffer[3000] = {0};
        auto valread = recv( client , buffer, 3000, 0);
        printf("Client: %s\n", buffer);
        //send(client , hello , hello_len, 0);
        

        if (!writeStrToClient(client, "HTTP/1.1 200 OK\r\n")){
            closesocket(client);
            continue;
        }

        char clen[40];
        sprintf(clen, "Content-length: %ld\r\n", fsize);
        if (!writeStrToClient(client, clen)){
            closesocket(client);
            continue;
        }

        if (!writeStrToClient(client, "Content-Type: text/html\r\n")){
            closesocket(client);
            continue;
        }

        if (!writeDataToClient(client, msg, fsize)){
            closesocket(client);
            continue;
        }

        printf("Sent message to client\n");
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}