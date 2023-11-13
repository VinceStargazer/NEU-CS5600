#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "get_env.h"

#define MAX_LINE_LENGTH 38

/**
 * This file tests page insertion, page update, and page replacement in a LRU cache
*/

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

    // Open input file
    FILE* file = fopen("message_ids.dat", "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    // Initialize a LRU cache
    lru_cache* cache = init_cache(cache_capacity);

    // Test inserting a page into cache by message retrieval from disk
    char line[MAX_LINE_LENGTH];
    int success = 1; // flag to indicate test result
    for (int i = 0; fgets(line, MAX_LINE_LENGTH, file) && i < cache_capacity; i++) {
        line[strcspn(line, "\n")] = '\0';
        if (retrieve_msg_from_disk(cache, message_size, line) == NULL) {
            printf("Error: Message ID not found in disk\n");
            success = 0;
        }
        if (cache->size != i + 1) {
            printf("Error: Incorrect cache size %zu\n", cache->size);
            success = 0;
        }
        if (strcmp(cache->sentinel->prev->key, line) != 0) {
            printf("Error: latest retrieved message is not at the tail of the cache\n");
            success = 0;
        }
    }
    if (success) {
        printf("Correct: page insertion\n");
    }

    // Test updating a page location by re-visiting it
    char* head_id = (char*)cache->sentinel->next->key;
    success = 1; // retore flag
    if (retrieve_msg_from_cache(cache, head_id) == NULL) {
        printf("Error: Message not found in cache\n");
        success = 0;
    }
    if (cache->size != cache_capacity) {
        printf("Error: Incorrect cache size %zu\n", cache->size);
        success = 0;
    }
    if (strcmp(cache->sentinel->prev->key, head_id) != 0) {
        printf("Error: latest re-visited message is not at the tail of the cache\n");
        success = 0;
    }
    if (success) {
        printf("Correct: page update\n");
    }

    // Test inserting a page into a full cache by message creation and replacing the least recently used one
    head_id = (char*)cache->sentinel->next->key;
    char* second_id = (char*)cache->sentinel->next->next->key;
    char* tail_id = (char*)cache->sentinel->prev->key;
    message_t* new_msg = create_msg(message_size, "Vince", "Jennifer", "That must be nice!", 0);
    store_msg(cache, message_size, new_msg);
    success = 1; // retore flag
    if (cache->size != cache_capacity) {
        printf("Error: Incorrect cache size %zu\n", cache->size);
        success = 0;
    }
    if (strcmp(cache->sentinel->prev->key, new_msg->id) != 0) {
        printf("Error: latest created message is not at the tail of the cache\n");
        success = 0;
    }
    if (strcmp(cache->sentinel->next->key, second_id) != 0 || strcmp(cache->sentinel->prev->prev->key, tail_id) != 0) {
        printf("Error: Inproper page replacement\n");
        success = 0;
    }
    node_t* temp = cache->sentinel->next;
    while (temp->key != NULL) {
        if (strcmp(temp->key, head_id) == 0) {
            printf("Error: LRU-Replaced page still exists in cache\n");
            success = 0;
        }
        temp = temp->next;
    }
    if (success) {
        printf("Correct: page replacement\n");
    }

    // Close the file and free memory
    fclose(file);
    free_cache(cache);
}