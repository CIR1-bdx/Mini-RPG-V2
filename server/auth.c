//
// Created by arno.dubois on 18-Dec-23.
//

void read_auth(){
    FILE* ptr = fopen("../debora.csv", "r");
    if (ptr == NULL) {
        perror(ptr);
        return;
    }

    char buffer[100];
    long long* read_array[1024][2];
    fscanf(ptr, "%s", buffer);
    char *token = strtok(buffer, ";");
    for (int i = 0; token != NULL; ++i) {
        printf("token : %s\n", token);
        read_array[i][0] = token;
        printf("%s -", read_array[i][0]);
        token = strtok(NULL, ";");
    }
    fscanf(ptr, "%s", buffer);
    token = strtok(buffer, ";");
    for (int i = 0; token != NULL; ++i) {
        printf("%s\n",token);
        read_array[i][1] = token;
        printf("> %s\n", read_array[i][1]);
        token = strtok(NULL, ";");
    }
    printf("tabl : %s\n", read_array[1][0]);

    for (int i = 0; i < 1024; ++i) {
        printf("%s -> %s\n", read_array[i][0],read_array[i][1]);
    }

    return;
}

