#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

void *handle_client(void *client_socket_ptr) {
    int client_socket = *((int *)client_socket_ptr);
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Recevoir les données du client
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Erreur de réception ou client déconnecté");
            break;
        }

        // Afficher le message reçu
        printf("message de %d ",client_socket);
        printf("Message du client: %s", buffer);

        // Vous pouvez ajouter ici une logique pour répondre au client si nécessaire

        // Effacer le tampon
        memset(buffer, 0, sizeof(buffer));
    }

    // Fermer le socket du client et terminer le thread
    close(client_socket);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilisation : %s <adresse_ip_du_serveur>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    pthread_t thread_id;

    // Créer le socket du serveur
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Initialiser la structure d'adresse du serveur
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);  // Adresse IP passée en argument
    server_address.sin_port = htons(PORT);

    // Lier le socket à l'adresse et au port spécifiés
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la liaison du socket");
        exit(EXIT_FAILURE);
    }

    // Mettre le serveur en mode écoute
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    while (1) {
        // Accepter la connexion d'un client
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
            perror("Erreur lors de l'acceptation de la connexion");
            continue;
        }

        printf("Nouvelle connexion acceptée de : %d\n",client_socket);

        // Créer un thread pour gérer le client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0) {
            perror("Erreur lors de la création du thread");
            continue;
        }

        // Détacher le thread pour permettre une terminaison automatique
        pthread_detach(thread_id);
    }

    // Fermer le socket du serveur (ce code ne sera jamais atteint dans cet exemple)
    close(server_socket);

    return 0;
}
