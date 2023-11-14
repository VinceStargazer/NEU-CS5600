#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "get_env.h"

#define SAMPLE_SIZE 1000
#define TEST_COUNT 1000
#define MAX_LINE_LENGTH 38

/**
 * This program tests the cache hit ratio by accessing COUNT random messages whose unique identifiers are stored in
 * messages_id.dat. It appends the testing result to the file evaluation.txt
*/

// Function to generate a random integer within a specific range [min, max]
int get_rand(int min, int max) {
    // Seed the random number generator with the current time

    // Calculate the random number within the specified range
    int randomNum = rand() % (max - min + 1) + min;
    return randomNum;
}

void execute(FILE* output, char ids[SAMPLE_SIZE][MAX_LINE_LENGTH], int message_size, int cache_capacity, int is_LRU) {
    // Initialize a LRU cache
    lru_cache* cache = init_cache(cache_capacity);
    
    // Loop COUNT times to calculate cache hits and misses
    int hits = 0;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < TEST_COUNT; i++) {
        // Fetch a random ID from the previous array
        int index = get_rand(0, SAMPLE_SIZE - 1);
        message_t* msg = retrieve_msg_from_cache(cache, ids[index]);
        if (msg != NULL) {
            hits++;
        } else {
            msg = retrieve_msg_from_disk(cache, message_size, ids[index], is_LRU);
        }
    }

    // Append test results
    char* replace_algo = is_LRU ? "LRU replacement" : "Random replacement";
    fprintf(output, "%s:\n", replace_algo);
    fprintf(output, "\tCache hits: %d\n", hits);
    fprintf(output, "\tCache misses: %d\n", TEST_COUNT - hits);
    fprintf(output, "\tCache hit ratio: %.2f%%\n", (double)hits / TEST_COUNT * 100);

    // Free cache memory
    free_cache(cache);
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

    char ids[SAMPLE_SIZE][MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    // Prepare for an array for random access
    for (int i = 0; fgets(line, MAX_LINE_LENGTH, input) && i < SAMPLE_SIZE; i++) {
        line[strcspn(line, "\n")] = '\0';
        // Copy the line to the array
        snprintf(ids[i], MAX_LINE_LENGTH, "%s", line);
    }

    fprintf(output, "---Random accessing %d messages %d times with cache capacity %d---\n", 
        SAMPLE_SIZE, TEST_COUNT, cache_capacity);
    execute(output, ids, message_size, cache_capacity, 1); // first, try LRU Replacement
    execute(output, ids, message_size, cache_capacity, 0); // then, try Random Replacement
    fprintf(output, "\n");

    // Close the files
    fclose(input);
    fclose(output);
}