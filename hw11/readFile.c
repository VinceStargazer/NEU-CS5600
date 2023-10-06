#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "polybius.h"

#define BATCH_COUNT 100

int main() {
    // Open text file
    FILE *file = fopen("output", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }
    // Initialize a queue
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    // Scan the input file and add words to queue
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *word = (char *)malloc(strlen(buffer) * sizeof(char));
        if (word == NULL) {
            perror("Failed to allocate word memory");
            return -1;
        }
        for (int i = 0; i < 100; i++) {
            if (buffer[i] == '\n') {
                break;
            }
            word[i] = buffer[i];
        }
        add2q(&queue, word);
    }

    int file_index = 1;
    while (queue.front != NULL) {
        char command[5000]; 
        strcpy(command, "./cipher -e ");   
        for (int i = 0; i < BATCH_COUNT && queue.front != NULL; i++) {
            char *word = (char *)popQ(&queue);
            strcat(command, word);
            strcat(command, " ");
            free(word);
        }
        command[strlen(command) - 1] = '\n';
        printf("%s", command);
        // FILE* pipe = popen(command, "r");
        // if (pipe == NULL) {
        //     perror("Failed to open pipe");
        //     return -1;
        // }
        // char buffer[100];
        // while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        //     // Process the output here (in this example, print it)
        //     printf("%s", buffer);
        // }
        // pclose(pipe);
        file_index++;
    }

    return 0;
}