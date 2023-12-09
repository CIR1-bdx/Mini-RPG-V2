#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilisation : %s <adresse_ip_du_serveur>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];

    // Créer le socket du client
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Initialiser la structure d'adresse du serveur
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);  // Adresse IP passée en argument
    server_address.sin_port = htons(PORT);

    // Connecter le client au serveur
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur. Vous pouvez commencer à envoyer des messages.\n");

    while (1) {
        // Lire l'entrée de l'utilisateur
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        // Envoyer le message au serveur
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
            break;
        }

        // Effacer le tampon
        memset(buffer, 0, sizeof(buffer));
    }

    // Fermer le socket du client
    close(client_socket);

    return 0;
}
