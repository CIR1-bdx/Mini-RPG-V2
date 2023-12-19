//
// Created by Victor on 26/10/2023.
//

#include "handle_client.h"
#include "time.h"
#include "../auth.c"

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID client_socket) {
#else
    void* handle_client(void* client_socket) {
#endif
    SOCKET socket = *(SOCKET*)client_socket;
    char buffer[BUFFER_SIZE];
    //srand(time(NULL));
//int random = rand();
    strcpy(buffer, "\02Avez vous un compte ?\n");
    send(socket, buffer, strlen(buffer), 0);
    int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes_received] = '\0';
    printf("Received message: %s\n", buffer);
    read_auth();


    while (1) {
        //printf("Running %d\n", random);
        int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received message: %s\n", buffer);
        if(strcmp(buffer, "ö\n") == 0) {
            printf("looooooooooooooooooooo\n");
        } else {
            printf("%d\n", strlen(buffer));
        }

// Send the message to all connected clients.
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != INVALID_SOCKET) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }
    }

    closesocket(socket);
    return 0;
}


