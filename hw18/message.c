#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "get_env.h"

const char* MSG_FORMAT_IN = 
    "{\n\t\"id\": \"%[^\"]\", \n\t\"time\": %ld, \n\t\"sender\": \"%[^\"]\", \n\t\"receiver\": \"%[^\"]\", \n\t\"content\": \"%[^\"]\", \"flag\": %d\n}\n";
const char* MSG_FORMAT_OUT = 
    "{\n\t\"id\": \"%s\", \n\t\"time\": %ld, \n\t\"sender\": \"%s\", \n\t\"receiver\": \"%s\", \n\t\"content\": \"%s\", \n\t\"flag\": %d\n}\n";

// Function to create a message object based on every given field
message_t* init_msg(size_t size, char* id, time_t time, char* sender, char* receiver, char* content, int flag) {
    message_t* message = (message_t*)malloc(size);
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
message_t* create_msg(size_t size, char* sender, char* receiver, char* content, int flag) {
    char* id = (char*)malloc(37 * sizeof(char));
    if (id == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    generate_uuid(id);
    message_t* message = init_msg(size, id, time(NULL), sender, receiver, content, flag);
    free(id);
    return message;
}

// Function to store a message object to both the cache and disk in a form similar to JSON
void store_msg(lru_cache* cache, size_t size, message_t* message, int is_LRU) {
    // first, store the message to cache
    if (is_LRU) {
        cache_put_LRU(cache, message->id, message);
    } else {
        cache_put_random(cache, message->id, message);
    }
    
    // second, store the message to disk
    FILE* file = fopen("messages.dat", "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, MSG_FORMAT_OUT, message->id, message->time, message->sender, 
        message->receiver, message->content, message->flag);
    fclose(file);
}

// Function to retrieve a message from the LRU cache. Returns NULL if message not found
message_t* retrieve_msg_from_cache(lru_cache* cache, char* id) {
    message_t* msg = (message_t*)cache_get(cache, id);
    return msg;
}

// Function to retrieve a message from the disk by the given ID. Returns NULL if message not found
message_t* retrieve_msg_from_disk(lru_cache* cache, size_t size, char* id, int is_LRU) {
    FILE* file = fopen("messages.dat", "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    message_t message;
    while (fscanf(file, MSG_FORMAT_IN, message.id, &message.time, message.sender, 
            message.receiver, message.content, &message.flag) == 6) {
        if (strcmp(message.id, id) == 0) { // ID hit
            message_t* msg = init_msg(size, message.id, message.time, message.sender, 
                message.receiver, message.content, message.flag);
            if (is_LRU) {
                cache_put_LRU(cache, msg->id, msg);
            } else {
                cache_put_random(cache, msg->id, msg);
            }
            fclose(file);
            return msg;
        }
    }
    fclose(file);
    return NULL;
}

// Function to retrieve a message from the LRU cache or the disk by the given ID. Returns NULL if message not found
message_t* retrieve_msg(lru_cache* cache, size_t size, char* id, int is_LRU) {
    // first, look for the message from cache
    message_t* msg = retrieve_msg_from_cache(cache, id);
    if (msg != NULL) { // ID hit
        printf("Found message in cache\n");
        return msg;
    }
    // second, look for the message from disk
    msg = retrieve_msg_from_disk(cache, size, id, is_LRU);
    if (msg != NULL) { // ID hit
        printf("Found message in disk\n");
    }
    return msg;
}