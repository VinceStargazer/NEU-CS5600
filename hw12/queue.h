#ifndef QUEUE_H
#define QUEUE_H

// Define node struct
typedef struct node_t {
    void *data;
    struct node_t *prev;
    struct node_t *next;
} node_t;

// Define queue struct
typedef struct queue_t {
    struct node_t *front;
    struct node_t *rear;
} queue_t;

// Define process struct
typedef struct process_t {
    int identifier;
    char *name;
    long cumulative_runtime;
    int priority;
} process_t;

// function to add one instance of an element to the end of the queue
// implement the queue as a doubly-linked list
void add2q(queue_t *queue, void *element);

// function to remove and return the data within the element at the front of the queue
void *popQ(queue_t *queue);

// function to remove and return the process with the highest priority in the queue
process_t *rmProcess(queue_t *queue);

// function to return the number of elements in the queue; 0 if the queue is empty
int qsize(queue_t *queue);

#endif