#include "./net/config.h"
int ID = 0;

typedef struct TableauCompte {
    int id;
    char pseudo[1024];
    char motDePasse[1024];
    struct TableauCompte *next;
} TableauCompte;

void ajouter(TableauCompte **list, char name[], char mdp[]) {
    TableauCompte *item = malloc(sizeof(TableauCompte));
    item->id = ID;
    ID++;
    strcpy(item->pseudo, name);
    strcpy(item->motDePasse, mdp);
    item->next = *list;
    *list = item;
}

char *pseudo_use(TableauCompte *list, char newName[]) {
    if (list == NULL)return NULL;
    while (list->next != NULL) {
        if (strcmp(list->pseudo, newName) == 0) {
            return list->motDePasse;
        }
        list = list->next;
    }
    if (strcmp(list->pseudo, newName) == 0) {
        return list->motDePasse;
    }
    return NULL;
}

int read_auth(char enter[], void *client_socket, int i) {
    SOCKET socket = *(SOCKET *) client_socket;

    char pseudo[1024] = "";
    for (int i = 0; i < strlen(enter)-1; ++i) {
        pseudo[i] = enter[i];
    }

    TableauCompte *listeCompte = NULL;
    FILE *ptr = fopen("../debora.csv", "r+");
    if (ptr == NULL) {
        perror(ptr);
        return 1;
    }

    char buffer[100];
    char *token;
    char name[1024];
    char mdp[1024] = "";
    while (fscanf(ptr, "%s", buffer) != -1) {
        token = strtok(buffer, ";");
        for (int i = 0; token != NULL; ++i) {
            if (i == 0) {
                strcpy(name, token);
            } else {
                strcpy(mdp, token);
            }
            token = strtok(NULL, ";");
        }
        ajouter(&listeCompte, name, mdp);
    }


    char *pointer = pseudo_use(listeCompte, pseudo);
    if (pointer == NULL) {
        char mdp[1024] = "";
        strcpy(name, pseudo);
        if (send(socket, "\02Nouveau compte\nEntrer mdp: ", strlen("\02Nouveau compte\nEntrer mdp: "), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
            return 1;
        }
        int bytes_received = recv(socket, mdp, sizeof(mdp), 0);
        if (bytes_received <= 0) {
            return 0;
        }
        ajouter(&listeCompte, name, mdp);
        fprintf(ptr,"");
        fprintf(ptr, "%s;%s", name, mdp);
    } else {
        while (1){
            char mdp[1024] = "";
            if (send(socket, "\02Entrez votre mdp: ", strlen("\02Entrez votre mdp: "), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
                return 1;
            }
            int bytes_received = recv(socket, mdp, sizeof(mdp), 0);
            if (bytes_received <= 0) {
                return 1;
            }
            char mot[1024] = "";
            for (int i = 0; i < strlen(mdp)-1; ++i) {
                mot[i] = mdp[i];
            }
            if (strcmp(mot, pointer) == 0) {
                break;
            }
        }
    }
    fclose(ptr);
    strcpy(listeClients[i].pseudo,pseudo);
    return 0;
}