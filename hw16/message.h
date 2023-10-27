#ifndef MESSAGE_H
#define MESSAGE_H

#include "uuid.h"
#include "linkedlist.h"

typedef struct {
    char id[37];
    time_t time;
    char sender[20];
    char receiver[20];
    char content[200];
    int flag;
} message_t;

message_t* init_msg(char* id, time_t time, char* sender, char* receiver, char* content, int flag);
message_t* create_msg(char* sender, char* receiver, char* content, int flag);
list_t* read_store_from_disk();
void save_store_to_disk(list_t* store);
void store_msg(list_t* store, message_t* message);
message_t* retrieve_msg(list_t* store, char* id);

#endif