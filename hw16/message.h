#ifndef MESSAGE_H
#define MESSAGE_H

#include "uuid.h"

typedef struct {
    char* id;
    time_t time;
    char* sender;
    char* receiver;
    char* content;
    int flag;
} message_t;

message_t* create_msg(char* sender, char* receiver, char* content, int flag);
void store_msg(message_t* message);
message_t* retrieve_msg(char* uuid);
void free_message(message_t* message);

#endif