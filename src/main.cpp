#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define PORT 8080
int main(){    
    auto hello = "Hello from server";
    int hello_len =  strlen(hello);



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
        send(client , hello , hello_len, 0);
        printf("Sent message to client\n");
        closesocket(client);
    }
    return 0;
}