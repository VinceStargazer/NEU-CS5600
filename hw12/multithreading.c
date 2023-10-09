#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"
#include "cipher.h"

#define BATCH_SIZE 100

typedef struct thread_t {
    char** words;
    int wordCount;
    int fileIndex;
} thread_t;

// Thread function to encode words
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
    printf("Count of %d: %d\n", data->fileIndex, data->wordCount);
    for (int i = 0; i < data->wordCount; i++) {
        printf("%s\n", data->words[i]);
        char* ciphertext = cipher(data->words[i]);
        if (ciphertext == NULL) {
            perror("cipher");
            pthread_exit(NULL);
        }
        fprintf(output, "%s\n", ciphertext);
        free(data->words[i]);
        free(ciphertext);
    }

    fclose(output);
    pthread_exit(NULL);
}

int main() {
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
        threadData[i].words = malloc(BATCH_SIZE * sizeof(char*));
        if (threadData[i].words == NULL) {
            perror("malloc");
            return -1;
        }
        int wordCount = 0;
        while (wordCount < BATCH_SIZE && queue.front != NULL) {
            threadData[i].words[wordCount++] = strdup(popQ(&queue));
        }
        threadData[i].wordCount = wordCount;
        threadData[i].fileIndex = i + 1;
        int rc = pthread_create(&threads[i], NULL, encryptWords, (void*)&threadData[i]);
        if (rc) {
            printf("Error: Return code from pthread_create() is %d\n", rc);
            return -1;
        }
        printf("Thread %d created\n", i);
    }

    // Wait for threads to complete
    for (int i = 0; i < batchCount; i++) {
        pthread_join(threads[i], NULL);
        free(threadData[i].words);
    }
    return 0;
}