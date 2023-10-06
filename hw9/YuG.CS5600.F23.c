/**
 * The thread behavior is not consistent in that two threads are writing to and reading from the global array synchronously, leading
 * to a race condition in which two threads compete against each other. Sometimes the reader thread reads data that have already got 
 * updated by the writer thread, and sometimes it reads data that haven't been reached nor initialized (still 0) by the other thread. 
 * To make sure the program is consistent and one thread can run independently of the other, it is recommended to use synchronization
 * mechanisms such as mutual exclusion to prevent multiple threads from accessing global variables concurrently.
 * Reference: StackOverFlow. (2017, March 18). Protecting global variable when using multiple threads. https://stackoverflow.com/questions/42879460/protecting-global-variable-when-using-multiple-threads
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ARRAY_SIZE 10

int globalData[ARRAY_SIZE];

void *writeToArray(void *data) {
    char *name = (char*)data;
 
    for (int i = 0; i < ARRAY_SIZE; i++) {
        usleep(50000);
        globalData[i] = i;
    }
 
    printf("Thread %s has completed writing data to array.\n", name);
    return NULL;
}

void *readFromArray(void *data) {
    char *name = (char*)data;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        usleep(50000);
        printf("%d ", globalData[i]);
    }

    printf("\nThread %s has completed reading data from array.\n", name);
    return NULL;
}

int main(void) {
    pthread_t th1, th2;
    pthread_create(&th1, NULL, writeToArray, "X");
    pthread_create(&th2, NULL, readFromArray, "Y");
    sleep(1);
    printf("====> Cancelling Thread Y with ID %lu!!\n", (long unsigned)th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread X with ID %lu!!\n", (long unsigned)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}