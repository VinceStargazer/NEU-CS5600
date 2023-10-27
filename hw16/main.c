#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: ./main [-c/-r] <args>\n");
        return -1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        if (argc != 6 || strcmp(argv[5], "0") != 0 && strcmp(argv[5], "1") != 0) {
            printf("Usage: ./main -c <sender> <receiver> <\"content\"> [1/0]\n");
            return -1;
        } else if (strlen(argv[2]) > 20 || strlen(argv[3]) > 20) {
            printf("Sender/Receiver max character: 20\n");
            return -1;
        } else if (strlen(argv[4]) > 200) {
            printf("Content max character: 20\n");
            return -1;
        }
        // Create mode: create a message and save it to message store
        message_t* message = create_msg(argv[2], argv[3], argv[4], atoi(argv[5]));
        store_msg(message);
        printf("A message of ID %s is created and stored.\n", message->id);
        free(message);
    } else if (strcmp(argv[1], "-r") == 0) {
        if (argc != 3) {
            printf("Usage: ./main -r <id>\n");
            return -1;
        } else if (strlen(argv[2]) != 36) {
            printf("ID length must be 36!\n");
            return -1;
        }
        // Retrieve mode: find and display a message by its identifier
        message_t* message = retrieve_msg(argv[2]);
        if (message != NULL) {
            printf("Message found:\n");
            printf("time: %s", ctime(&message->time));
            printf("sender: %s\n", message->sender);
            printf("receiver: %s\n", message->receiver);
            printf("content: %s\n", message->content);
            printf("flag: %d\n", message->flag);
            free(message);
        } else {
            printf("Message not found!\n");
        }
    } else {
        printf("Usage: ./main [-c/-r] <args>");
        return -1;
    }

    return 0;
}