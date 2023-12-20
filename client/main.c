#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <process.h>
#pragma comment(lib, "Ws2_32.lib")
#else

#include <arpa/inet.h>
#include <pthread.h>

#endif

#define IP "127.0.0.1"
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

void *recive_msg(void *client_socket_ptr) {
    int client_socket = *((int *) client_socket_ptr);
    char serv_buffer[1024];

    recv(client_socket, serv_buffer, sizeof(serv_buffer), 0);
    printf("%s\n", serv_buffer);
    memset(serv_buffer, 0, sizeof(serv_buffer));
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
    WSADATA wsaData;
#endif
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];
#ifdef _WIN32
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }
#ifdef _WIN32
        memset(&server_address, '\0', sizeof(server_address));
#else
    memset(&server_address, 0, sizeof(server_address));
#endif

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IP);  // Adresse IP passée en argument
    server_address.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }


    printf("Connecté au serveur. Vous pouvez commencer à envoyer des messages.\n");


    while (1) {
        char serv_buffer[1024] = "";
        int bytes_received = recv(client_socket, serv_buffer, sizeof(serv_buffer), 0);
        if (bytes_received <= 0) {
            break;
        }
        printf("%s\n", serv_buffer);
        if (serv_buffer[0] == '\02') {
            printf("Client: ");
            fgets(buffer, sizeof(buffer), stdin);

            if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
                continue;
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }

#ifdef _WIN32
    closesocket(client_socket);
    WSACleanup();
#else
    close(client_socket);
#endif

    return 0;
}
