#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

// Function to create a new node
node_t* init_node(void* data) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Function to create an empty list
list_t* init_list() {
    list_t* list = (list_t*)malloc(sizeof(list_t));
    if (list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    list->length = 0;
    list->sentinel = init_node(NULL);
    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    return list;
}

// Function to add a node to the front of the list
void add_first(list_t* list, node_t* node) {
    node_t* head = list->sentinel->next;
    node->next = head;
    head->prev = node;
    list->sentinel->next = node;
    node->prev = list->sentinel;
    list->length++;
}

// Function to add a node to the rear of the list
void add_last(list_t* list, node_t* node) {
    node_t* tail = list->sentinel->prev;
    tail->next = node;
    node->prev = tail;
    list->sentinel->prev = node;
    node->next = list->sentinel;
    list->length++;
}

// Function to remove the node from the front of the list and return its data
void* poll_first(list_t* list) {
    if (list->length == 0) {
        return NULL;
    }
    node_t* head = list->sentinel->next;
    head->next->prev = list->sentinel;
    list->sentinel->next = head->next;
    list->length--;
    void* result = head->data;
    free(head);
    return result;
}

// Function to remove the node from the rear of the list and return its data
void* poll_last(list_t* list) {
    if (list->length == 0) {
        return NULL;
    }
    node_t* tail = list->sentinel->prev;
    tail->prev->next = list->sentinel;
    list->sentinel->prev = tail->prev;
    list->length--;
    void* result = tail->data;
    free(tail);
    return result;
}

// Function to remove a given node from the list
void remove_node(list_t* list, node_t* node) {
    // Empty list or attempt to remove sentinel
    if (list->length == 0 || node->data == NULL) {
        return;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    list->length--;
}

// Function to clear all elements of the list and free all allocated memory
void free_list(list_t* list) {
    while (list->length > 0) {
        poll_first(list);
    }
    free(list->sentinel);
    free(list);
}
