#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

int main() {
    // Open .env and find global variables
    FILE* env_file = fopen(".env", "r");
    if (env_file == NULL) {
        perror("Error opening .env file");
        return -1;
    }
    int message_size = 0, cache_capacity = 0;
    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, env_file) && (message_size == 0 || cache_capacity == 0)) {
        char* equals = strchr(line, '=');
    }

    fclose(env_file);
    // Initialize a LRU cache
    lru_cache* cache = init_cache(100);
    char* command = NULL;
    size_t bufsize = 0;
    size_t characters;

    while (1) {
        printf("Enter a command: ");

        // Read a line from stdin
        characters = getline(&command, &bufsize, stdin);
        
        // Remove the newline character from the end of the input
        if (characters > 0 && command[characters - 1] == '\n') {
            command[characters - 1] = '\0';
        }
        
        // Check for exit condition
        if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0) {
            printf("Exiting the program.\n");
            break;
        }

        if (strcmp(command, "c") == 0 || strcmp(command, "create") == 0) {
            // Create mode
            char sender[20];
            char receiver[20];
            char content[200];
            int flag = -1;

            printf("Enter the message sender: ");
            scanf(" %20[^\n]%*c", sender);

            printf("Enter the message receiver: ");
            scanf(" %20[^\n]%*c", receiver);

            printf("Enter the message content: ");
            scanf(" %200[^\n]%*c", content);

            printf("Was the message delivered? (1/0): ");
            scanf("%d%*c", &flag);

            if (flag != 1 && flag != 0) {
                printf("Invalid answer!\n");
                continue;
            }
            
            message_t* message = create_msg(sender, receiver, content, flag);
            store_msg(cache, message);
            printf("A message of ID %s is created and stored.\n", message->id);
        } else if (strcmp(command, "r") == 0 || strcmp(command, "retrieve") == 0) {
            // Retrieve mode
            char id[36];
            printf("Enter the message ID: ");
            scanf(" %36[^\n]%*c", id);

            message_t* message = retrieve_msg(cache, id);
            if (message == NULL) {
                printf("Message not found!\n");
            } else {
                printf("Message found:\n");
                printf("\ttime: %s", ctime(&message->time));
                printf("\tsender: %s\n", message->sender);
                printf("\treceiver: %s\n", message->receiver);
                printf("\tcontent: %s\n", message->content);
                printf("\tflag: %d\n", message->flag);
            }
        } else {
            printf("Command not found!\n");
        }
    }

    
    // Free allocated memory and serialize to disk file
    free(command);

    return 0;
}