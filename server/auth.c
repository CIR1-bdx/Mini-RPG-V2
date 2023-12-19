//
// Created by arno.dubois on 18-Dec-23.
//

void read_auth(){
    FILE* ptr = fopen("../debora.csv", "r");
    if (ptr == NULL) {
        perror(ptr);
        return;
    }

    char buf[100];
    char buf1[100];
    while (fscanf(ptr, "%s;%s ", buf,buf1) == 1)
        printf("%s\n", buf);
    return;
}

