#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_env_var(char* target) {
    // Open .env and locate global variables
    FILE* file = fopen(".env", "r");
    if (file == NULL) {
        perror("Error opening .env file");
        return -1;
    }
    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, file)) {
        // parse the input line by '=' to read key and value
        char* equals = strchr(line, '=');
        if (equals != NULL) {
            *equals = '\0';
            if (strcmp(line, target) == 0) {
                fclose(file);
                return atoi(equals + 1);
            }
        }
    }
    fclose(file);
    return 0;
}
