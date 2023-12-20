//
// Created by Victor on 26/10/2023.
//

#include "handle_client.h"
#include "time.h"
#include "../auth.c"

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID client_socket) {
#else

void *handle_client(void *client_socket) {
#endif
    SOCKET socket = *(SOCKET *) client_socket;
    char buffer[BUFFER_SIZE];
    while (1){
        if (send(socket, "\02Quel est votre pseudo", strlen("\02rQuel est votre pseudo"), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
            return 0;
        }
        int bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            return 0;
        }
        if (strlen(buffer) <= 4) { continue; }
        break;
    }
    if (read_auth(buffer,client_socket) == 1){
        closesocket(socket);
        printf("Removed 1 client, destroyed 1 thread\n");
        return 0;
    }

    closesocket(socket);
    printf("Removed 1 client, destroyed 1 thread\n");
    return 0;
}


