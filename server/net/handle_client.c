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
    char pseudo[32];
    //srand(time(NULL));
//int random = rand();




    while (1) {
        strcpy(buffer, "\02Avez vous un compte ?(oui/non)\n");
        send(socket, buffer, strlen(buffer), 0);

        int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';

        if(strcmp(buffer, "oui\n") == 0){
            while(1) {
                strcpy(buffer, "\02Quelle est votre pseudo ?\n");
                send(socket, buffer, strlen(buffer), 0);

                bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    break;
                }
                buffer[bytes_received] = '\0';

                /*   int i,j;
                *   i = 0;
                *   while(i<strlen(buffer)){
                *       if (buffer[i]==";") {
                *           for (j=i; j<strlen(buffer); j++)
                *               buffer[j]=buffer[j+1];
                *       } else i++;
                *   }

                * if(pseudoExist(buffer)) {
                 * strcpy(pseudo, buffer);
                 * break;
                 * } else {
                 *  printf("Ce pseudo n'existe pas\n");
                 * }
                 *
                 */
                break;

            }
            /* while(1) {
            *      strcpy(buffer, "\02Quelle est votre mot de passe ?\n");
            *      send(socket, buffer, strlen(buffer), 0);
            *
            *      bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
            *      if (bytes_received <= 0) {
            *             break;
            *      }}
            *      buffer[bytes_received] = '\0';
            *
             *   int i,j;
                *   i = 0;
                *   while(i<strlen(buffer)){
                *       if (buffer[i]==";") {
                *           for (j=i; j<strlen(buffer); j++)
                *               buffer[j]=buffer[j+1];
                *       } else i++;
                *   }
                *
            *      if(mdpCorrect(buffer)){
             *          break;
             *      } else {
             *          printf("Mot de passe incorrecte, veuillez reéssayer\n");
             *      }
            *
            * }
            */

        } else if(strcmp(buffer, "non\n") == 0){
            while(1) {
                strcpy(buffer, "\02Quelle pseudo voulez vous utiliser ?\n");
                send(socket, buffer, strlen(buffer), 0);

                bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    break;
                }
                buffer[bytes_received] = '\0';

                /*   int i,j;
                *   i = 0;
                *   while(i<strlen(buffer)){
                *       if (buffer[i]==";") {
                *           for (j=i; j<strlen(buffer); j++)
                *               buffer[j]=buffer[j+1];
                *       } else i++;
                *   }

                * if(pseudoExist(buffer) {
                 *      printf("Ce pseudo existe déjà, veuillez en choisir un autre\n");
                 * } else {
                 *      break;
                 * }
                 */
                break;
            }
            /*
             * strcpy(buffer, "\02Quelle mot de passe voulez vous utiliser ?\n");
             * send(socket, buffer, strlen(buffer), 0);
             *
             * bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
             * if (bytes_received <= 0) {
             *      break;
             *  }
             *  buffer[bytes_received] = '\0';
             *
             *   int i,j;
                *   i = 0;
                *   while(i<strlen(buffer)){
                *       if (buffer[i]==";") {
                *           for (j=i; j<strlen(buffer); j++)
                *               buffer[j]=buffer[j+1];
                *       } else i++;
                *   }
             *  createNewUser(pseudo, mdp);
             */

        } else {
            strcpy(buffer, "Entrer invalide, veuillez reéssayer\n");
            send(socket, buffer, strlen(buffer), 0);
        }


        //read_auth();

// Send the message to all connected clients.
        /*for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != INVALID_SOCKET) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }*/
    }


    closesocket(socket);
    printf("Removed 1 client, destroyed 1 thread\n");
    return 0;
}


