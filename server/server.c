#include "net/config.h"
#include "server.h"


#include "net/handle_client.h"


int start_server() {
    SOCKET server_socket, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);

#ifdef _WIN32
    WSADATA wsa_data;
    typedef int socklen_t;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to initialize.\n");
        return 1;
    }
#endif
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        listeClients[i].socket = INVALID_SOCKET;
        strcpy(listeClients[i].pseudo,"");
    }
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    int clientID;

    while (1) {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            printf("(%d)%s\n",i,listeClients[i].pseudo);
        }
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addr_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Add the new client to the clients array and start a new thread for the client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (listeClients[i].socket == INVALID_SOCKET) {
                listeClients[i].socket = new_socket;
                printf("added client\n");

#ifdef _WIN32 // Création du thread sur windows
                HANDLE threadHandle;
                threadHandle = CreateThread(NULL, 0, handle_client, &clients[i], 0, NULL);
                if (threadHandle != NULL) {
                    CloseHandle(threadHandle);
                    printf("Created 1 thread\n");
                }
#else // Création du thread sur unix
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, handle_client, &i);
                pthread_detach(thread_id); // To automatically reclaim thread resources
#endif

                break;
            }
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}