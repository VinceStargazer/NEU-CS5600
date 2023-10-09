/*
* Guoqing Yu / CS5600 / Northeastern University
* Collaborator: Yun-Chi Pang, I-Hsin(Jessica) Li
* Fall 2023 / Sep 22, 2023
*/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void add2q(queue_t* queue, void* element) {
    // 1. create a new node to hole the element passed in
    // node is a pointer pointing to the mempry block which holds a node_t struct
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = element;
    node->prev = NULL;
    node->next = NULL;

    // 2. push the new node into queue
    // if q is empty, make front and rear both to this new node
    if (queue->front == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        // if q is not empty, append this new node to the rear of the current q
        queue->rear->next = node; // point q rear to new node
        node->prev = queue->rear; // point new node prev to q rear
        queue->rear = node; // update q rear pointer
    }
};

void* popQ(queue_t* queue) {
    // if queue is empty return NULL
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty. Cannot pop.\n");
        return NULL;
    }
    // if queue is not empty
    // 1. remove the front node and get the node's data
    node_t *temp = queue->front;
    void *data = temp->data;
    // 2. update the front pointer of the queue
    queue->front = queue->front->next;
    // 3. if the queue is empty now (popped the only node), update rear as well
    // rear was pointing to the only node, now update it to NULL
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    // 4. free te temp node and return data
    free(temp);
    return data;
};

process_t* rmProcess(queue_t* queue) {
    // if queue is empty return NULL
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty. Cannot remove process.\n");
        return NULL;
    }
    // if queue is not empty
    // 1. traverse the queue to find the top-priority process
    // 
    node_t *current = queue->front;
    node_t *highestPriority = queue->front;
    // keep shifting the curr pointer to right, if curr priority has a higher priority
    while (current != NULL) {
        process_t *currentProcess = (process_t *)current->data;
        process_t *highestPriorityProcess = (process_t *)highestPriority->data;

        if (currentProcess->priority > highestPriorityProcess->priority) {
            highestPriority = current;
        }
        current = current->next;
    }
    // remove the process
    void *data = highestPriority->data;
    // if highestPriority is NOT the first node in queue
    if (highestPriority->prev != NULL) {
        highestPriority->prev->next = highestPriority->next;
    } else {
        // if highestPriority is the first node in queue -> update q's front pointer to the next node
        queue->front = queue->front->next;
    }
    // if highestPriority is NOT the last node in queue
    if (highestPriority->next != NULL) {
        highestPriority->next->prev = highestPriority->prev;
    } else {
        // if highestPriority is the last node in queue -> update q's rear pointer to the prev node
        queue->rear = queue->rear->prev;
    }
    // free the node removed
    free(highestPriority);
    return (process_t *)data;
};

int qsize(queue_t* queue) {
    // traverse through the queue (doubly-linked list) to count the number of nodes
    int count = 0;
    node_t *curr = queue->front;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
};