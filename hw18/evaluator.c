#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "get_env.h"

#define COUNT 1000
#define MAX_LINE_LENGTH 38

// Function to generate a random integer within a specific range [min, max]
int get_rand(int min, int max) {
    // Seed the random number generator with the current time

    // Calculate the random number within the specified range
    int randomNum = rand() % (max - min + 1) + min;
    return randomNum;
}

int main() {
    // Get environment variables
    int message_size = get_env_var("MESSAGE_SIZE");
    if (message_size == 0) {
        perror("Message size not found in .env file");
        return -1;
    }
    int cache_capacity = get_env_var("CACHE_CAPACITY");
    if (cache_capacity == 0) {
        perror("Cache capacity not found in .env file");
        return -1;
    }

    // Open input and output files
    FILE* input = fopen("message_ids.dat", "r");
    if (input == NULL) {
        perror("fopen");
        return -1;
    }
    FILE* output = fopen("evaluation.txt", "a");
    if (output == NULL) {
        perror("fopen");
        return -1;
    }

    // Initialize a LRU cache
    lru_cache* cache = init_cache(cache_capacity);

    char ids[COUNT][MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    // Prepare for an array for random access
    for (int i = 0; fgets(line, MAX_LINE_LENGTH, input) && i < COUNT; i++) {
        line[strcspn(line, "\n")] = '\0';
        // Copy the line to the array
        snprintf(ids[i], MAX_LINE_LENGTH, "%s", line);
    }
    fclose(input);
    
    fprintf(output, "---Testing random accessing %d messages with a cache of capacity %d---\n", COUNT, cache_capacity);
    // Loop COUNT times to calculate cache hits and misses
    int hits = 0;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < COUNT; i++) {
        // Fetch a random ID from the previous array
        int index = get_rand(0, COUNT - 1);
        message_t* msg = retrieve_msg_from_cache(cache, ids[index]);
        if (msg != NULL) {
            hits++;
        } else {
            msg = retrieve_msg_from_disk(cache, message_size, ids[index]);
        }
    }

    fprintf(output, "Cache hits: %d\n", hits);
    fprintf(output, "Cache misses: %d\n", COUNT - hits);
    fprintf(output, "Cache hit ratio: %.2f%%\n", (double)hits / COUNT * 100);

    // Close the file and free memory
    fclose(output);
    free_cache(cache);
}