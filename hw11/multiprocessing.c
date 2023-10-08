#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "queue.h"

#define BATCH_SIZE 100

void encryptWords(char* words[], int wordCount, int fileIndex) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid;
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: close write end of the pipe
        close(pipefd[1]);
        // Redirect stdin to read from the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        // Redirect stdout to write to a file
        char fileName[20];
        sprintf(fileName, "output_batch_%d.txt", fileIndex);
        freopen(fileName, "w", stdout);
        execl("./cipher", "./cipher", "-e", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: close read end of the pipe
        close(pipefd[0]);
        // Write data to the pipe
        for (int i = 0; i < wordCount; i++) {
            if (write(pipefd[1], words[i], strlen(words[i])) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            if (i < wordCount - 1 && write(pipefd[1], "\n", 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        close(pipefd[1]);
        wait(NULL);
    }
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
    // Scan the input file and add words to queue
    char buffer[100];
    int totalWords = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *word = (char *)malloc(strlen(buffer) * sizeof(char));
        if (word == NULL) {
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
        add2q(&queue, word);
        totalWords++;
    }

    int batchCount = (totalWords + BATCH_SIZE - 1) / BATCH_SIZE;
    for (int i = 0; i < batchCount; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        char* words[BATCH_SIZE];
        int wordCount = 0;
        while (wordCount < BATCH_SIZE && queue.front != NULL) {
            words[wordCount++] = (char *)popQ(&queue);
        }
        if (pid == 0) {
            // Call cipher program only from child process
            encryptWords(words, wordCount, i + 1);
            return 0;
        }
    }
    
    for (int i = 0; i < batchCount; i++) {
        wait(NULL);
    }
    return 0;
}