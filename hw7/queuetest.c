/*
* Guoqing Yu / CS5600 / Northeastern University
* Collaborator: Yun-Chi Pang, I-Hsin(Jessica) Li
* Fall 2023 / Sep 22, 2023
*/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main() {
    // 1. create a new queue
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    printf("Initial queue size: %d\n\n", qsize(&queue));

    // 2. create a add some processes to the queue
    process_t process1 = {1, "Process A", 100, 2};
    process_t process2 = {2, "Process B", 200, 1};
    process_t process3 = {3, "Process C", 150, 3};
    process_t process4 = {4, "Process D", 50, 2};

    add2q(&queue, &process1);
    printf("Added Process A to the queue. Queue size: %d\n", qsize(&queue));

    add2q(&queue, &process2);
    printf("Added Process B to the queue. Queue size: %d\n", qsize(&queue));

    add2q(&queue, &process3);
    printf("Added Process C to the queue. Queue size: %d\n", qsize(&queue));

    add2q(&queue, &process4);
    printf("Added Process D to the queue. Queue size: %d\n\n", qsize(&queue));

    // 3. print contents of the queue
    printf("Contents of the queue:\n");
    node_t *current = queue.front;
    while (current != NULL) {
        process_t *currentProcess = (process_t *)current->data;
        printf("Process %s (ID: %d, Priority: %d, Runtime: %ld)\n", 
               currentProcess->name, currentProcess->identifier, 
               currentProcess->priority, currentProcess->cumulative_runtime);
        current = current->next;
    }
    printf("\n");

    // 4. remove and print the process with the highest priority
    process_t *highestPriorityProcess = rmProcess(&queue);
    if (highestPriorityProcess != NULL) {
        printf("Removed process with highest priority: %s (ID: %d, Priority: %d, Runtime: %ld)\n",
               highestPriorityProcess->name, highestPriorityProcess->identifier,
               highestPriorityProcess->priority, highestPriorityProcess->cumulative_runtime);
    }
    printf("Queue size after removing one process: %d\n\n", qsize(&queue));

    return 0;
}