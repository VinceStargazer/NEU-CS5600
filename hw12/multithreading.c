#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "cipher.h"

#define BATCH_SIZE 100

typedef struct thread_t {
    queue_t* queue;
    int fileIndex;
} thread_t;

sem_t semaphore;

// Thread function to pop words from queue and encode words
void* encryptWords(void* arg) {
    thread_t* data = (thread_t*)arg;
    char fileName[20];
    sprintf(fileName, "output_batch_%d.txt", data->fileIndex);
    // Create output file
    FILE* output = fopen(fileName, "w");
    if (output == NULL) {
        perror("Failed to create output file");
        pthread_exit(NULL);
    }
    for (int i = 0; i < BATCH_SIZE && data->queue->front != NULL; i++) {
        sem_wait(&semaphore);
        char* word = popQ(data->queue);
        sem_post(&semaphore);
        char* ciphertext = cipher(word);
        free(word);
        if (ciphertext == NULL) {
            perror("cipher");
            pthread_exit(NULL);
        }
        fprintf(output, "%s\n", ciphertext);
        free(ciphertext);
    }

    fclose(output);
    pthread_exit(NULL);
}

int main() {
    sem_init(&semaphore, 0, 1);
    // Open text file
    FILE *file = fopen("random_words.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }
    // Initialize a queue
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    
    char buffer[100];
    int totalWords = 0;
    // Scan the input file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* word = (char*)malloc(strlen(buffer) * sizeof(char));
        if (word == NULL) {
            fclose(file);
            perror("Failed to allocate word memory");
            return -1;
        }
        for (int i = 0; i < 100; i++) {
            if (buffer[i] == '\n') {
                word[i] = '\0';
                break;
            }
            word[i] = buffer[i];
        }
        // Add word to queue
        add2q(&queue, word);
        totalWords++;
    }
    fclose(file);

    int batchCount = (totalWords + BATCH_SIZE - 1) / BATCH_SIZE;
    // Create as many threads as batch files
    pthread_t threads[batchCount];
    thread_t threadData[batchCount];
    for (int i = 0; i < batchCount; i++) {
        threadData[i].queue = &queue;
        threadData[i].fileIndex = i + 1;
        int rc = pthread_create(&threads[i], NULL, encryptWords, (void*)&threadData[i]);
        if (rc) {
            printf("Error: Return code from pthread_create() is %d\n", rc);
            return -1;
        }
    }

    for (int i = 0; i < batchCount; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&semaphore);
    return 0;
}