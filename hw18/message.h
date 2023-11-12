#ifndef MESSAGE_H
#define MESSAGE_H

#include "uuid.h"
#include "LRUcache.h"

typedef struct {
    char id[37];
    time_t time;
    char sender[20];
    char receiver[20];
    char content[200];
    int flag;
    char padding[];
} message_t;

message_t* init_msg(size_t size, char* id, time_t time, char* sender, char* receiver, char* content, int flag);
message_t* create_msg(size_t size, char* sender, char* receiver, char* content, int flag);
void store_msg(lru_cache* cache, size_t size, message_t* message);
message_t* retrieve_msg(lru_cache* cache, size_t size, char* id);

#endif