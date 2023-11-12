#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* input = fopen("message_ids.dat", "a");
    if (input == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    FILE* output = fopen("evaluation.txt", "w");
    char id[36];
    for (int i = 0; fscanf(input, "%s\n", id) && i < 1000; i++) {
        
    }
    fclose(input);
    fclose(output);
}