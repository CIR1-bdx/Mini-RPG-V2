#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <pthread.h>

#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>

int main() {

    WSADATA wsaData;
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    WSAStartup(MAKEWORD(2,2), &wsaData);
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("10.40.216.30");

    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        perror("Pas connecté");
        exit(EXIT_FAILURE);
    }

//    printf("Enter your login : ");
//    fgets(buffer, 1024, stdin);
//    send(clientSocket, buffer, strlen(buffer), 0);
//
//    printf("Enter your password : ");
//    fgets(buffer, 1024, stdin);
//    send(clientSocket, buffer, strlen(buffer), 0);

    while(1) {
        printf("Enter a message : ");
        fgets(buffer, 1024, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);

//        recv(clientSocket,buffer,1024,0);
//        printf("Message from server : %s\n",buffer);
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}