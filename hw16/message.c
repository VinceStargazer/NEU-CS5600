#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

const char* MSG_FORMAT_IN = 
    "{\n\t\"id\": \"%[^\"]\", \n\t\"time\": %ld, \n\t\"sender\": \"%[^\"]\", \n\t\"receiver\": \"%[^\"]\", \n\t\"content\": \"%[^\"]\", \"flag\": %d\n}\n";
const char* MSG_FORMAT_OUT = 
    "{\n\t\"id\": \"%s\", \n\t\"time\": %ld, \n\t\"sender\": \"%s\", \n\t\"receiver\": \"%s\", \n\t\"content\": \"%s\", \n\t\"flag\": %d\n}\n";

message_t* create_msg(char* sender, char* receiver, char* content, int flag) {
    message_t* message = (message_t*)malloc(sizeof(message_t));
    if (message == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    message->id = (char*)malloc(37 * sizeof(char));
    if (message->id == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    generate_uuid(message->id);
    message->time = time(NULL);
    message->sender = strdup(sender);
    message->receiver = strdup(receiver);
    message->content = strdup(content);
    message->flag = flag;
    return message;
}

void store_msg(message_t* message) {
    FILE* file = fopen("messages.dat", "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, MSG_FORMAT_OUT, message->id, message->time, message->sender, 
        message->receiver, message->content, message->flag);
    fclose(file);
}

message_t* retrieve_msg(char* uuid) {
    FILE* file = fopen("messages.dat", "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    message_t* message = create_msg("", "", "", 0);
    while (fscanf(file, MSG_FORMAT_IN, message->id, &message->time, message->sender, 
            message->receiver, message->content, &message->flag) == 6) {
        if (strcmp(message->id, uuid) == 0) {
            fclose(file);
            return message;
        }
    }
    free_message(message);
    fclose(file);
    return NULL;
}

void free_message(message_t* message) {
    free(message->id);
    free(message->sender);
    free(message->receiver);
    free(message->content);
    free(message);
}
