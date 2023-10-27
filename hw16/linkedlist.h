#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node_t {
    void* data;
    struct node_t* prev;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* sentinel;
    size_t length;
} list_t;

node_t* init_node(void* data);
list_t* init_list();
void add_first(list_t* list, node_t* node);
void add_last(list_t* list, node_t* node);
void* poll_first(list_t* list);
void* poll_last(list_t* list);
void remove_node(list_t* list, node_t* node);
void free_list(list_t* list);

#endif