#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

const char* MSG_FORMAT_IN = 
    "{\n\t\"id\": \"%[^\"]\", \n\t\"time\": %ld, \n\t\"sender\": \"%[^\"]\", \n\t\"receiver\": \"%[^\"]\", \n\t\"content\": \"%[^\"]\", \"flag\": %d\n}\n";
const char* MSG_FORMAT_OUT = 
    "{\n\t\"id\": \"%s\", \n\t\"time\": %ld, \n\t\"sender\": \"%s\", \n\t\"receiver\": \"%s\", \n\t\"content\": \"%s\", \n\t\"flag\": %d\n}\n";

// Function to create a message object based on every given field
message_t* init_msg(char* id, time_t time, char* sender, char* receiver, char* content, int flag) {
    message_t* message = (message_t*)malloc(sizeof(message_t));
    if (message == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(message->id, id);
    message->time = time;
    strcpy(message->sender, sender);
    strcpy(message->receiver, receiver);
    strcpy(message->content, content);
    message->flag = flag;
    return message;
}

// Function to create the unique ID and current timestamp for init_msg()
message_t* create_msg(char* sender, char* receiver, char* content, int flag) {
    char* id = (char*)malloc(37 * sizeof(char));
    if (id == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    generate_uuid(id);
    message_t* message = init_msg(id, time(NULL), sender, receiver, content, flag);
    free(id);
    return message;
}

// Function to read and deserialize messages from disk file into a linked list
list_t* read_store_from_disk() {
    list_t* store = init_list();
    FILE* file = fopen("messages.dat", "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    message_t message;
    while (fscanf(file, MSG_FORMAT_IN, message.id, &message.time, message.sender, 
            message.receiver, message.content, &message.flag) == 6) {
        message_t* msg_pt = init_msg(message.id, message.time, message.sender, 
                message.receiver, message.content, message.flag);
        add_last(store, init_node(msg_pt));
    }
    fclose(file);
    return store;
}

// Function to serialize a linked list into a data file on disk
void save_store_to_disk(list_t* store) {
    FILE* file = fopen("messages.dat", "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (store->length > 0) {
        message_t* message = poll_first(store);
        fprintf(file, MSG_FORMAT_OUT, message->id, message->time, message->sender, 
            message->receiver, message->content, message->flag);
        free(message);
    }
    fclose(file);
}

// Function to store a message object in the current directory in a form similar to JSON
void store_msg(list_t* store, message_t* message) {
    add_first(store, init_node(message));
}

// Function to retrieve a message from the message store by the given ID. Returns NULL if message not found
message_t* retrieve_msg(list_t* store, char* id) {
    node_t* node = store->sentinel->next;
    while (node->data != NULL) {
        node_t* next = node->next;
        message_t* message = (message_t*)node->data;
        // ID hit
        if (strcmp(message->id, id) == 0) {
            remove_node(store, node);
            add_first(store, node);
            return message;
        }
        node = next;
    }
    return NULL;
}
