#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

int main() {
    char sender[] = "Tom";
    char receiver[] = "Amy";
    char content[] = "How are you?";
    int flag = 1;
    message_t* message = create_msg(sender, receiver, content, flag);
    char* uuid = strdup(message->id);
    store_msg(message);
    free_message(message);

    message = retrieve_msg(uuid);
    if (message != NULL) {
        printf("id: %s\n", message->id);
        printf("time: %s", ctime(&message->time));
        printf("sender: %s\n", message->sender);
        printf("receiver: %s\n", message->receiver);
        printf("content: %s\n", message->content);
        printf("flag: %d\n", message->flag);
        free_message(message);
    } else {
        printf("Message not found!\n");
    }
    free(uuid);
    return 0;
}