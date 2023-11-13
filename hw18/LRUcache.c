#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LRUcache.h"

/**
 * I use a doubly linked list (DDL) and a hash table to implement LRU cache, which enables page lookup, insertion, update,
 * and removal to be done in O(1) time complexity. In this data structure, the least recently used page is placed at the 
 * head of the DDL while the most recently used one is at its tail. There are two replacement algorithms that will be 
 * triggered if a page is placed into the cache but no "slot" is found: Random Replacement (cache_put_random) and LRU (cache_put_LRU)
*/

// Function to generate a random integer within a specific range [min, max]
int get_rand_int(int min, int max) {
    // Calculate the random number within the specified range
    int randomNum = rand() % (max - min + 1) + min;
    return randomNum;
}

// Function to create a new node
node_t* init_node(void* key, void* value) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->value = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Function to create a LRU cache
lru_cache* init_cache(size_t capacity) {
    lru_cache* cache = (lru_cache*)malloc(sizeof(lru_cache));
    if (cache == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    cache->size = 0;
    cache->capacity = capacity;
    cache->sentinel = init_node(NULL, NULL);
    cache->sentinel->next = cache->sentinel;
    cache->sentinel->prev = cache->sentinel;
    cache->map = init_map();
    return cache;
}

void cache_remove(lru_cache* cache, node_t* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void cache_add(lru_cache* cache, node_t* node) {
    node_t* orig = cache->sentinel->prev;
    orig->next = node;
    node->prev = orig;
    node->next = cache->sentinel;
    cache->sentinel->prev = node;
}

void cache_update(lru_cache* cache, node_t* node) {
    cache_remove(cache, node);
    cache_add(cache, node);
}

// Function to retrieve a data entry from cache by key
void* cache_get(lru_cache* cache, void* key) {
    node_t* node = (node_t*)map_get(cache->map, key);
    if (node == NULL) {
        return NULL;
    }
    cache_update(cache, node);
    return node->value;
}

// Function to insert a key-value entry into the cache
void cache_put(lru_cache* cache, void* key, void* value) {
    node_t* node = map_get(cache->map, key);
    if (node == NULL) {
        // when the key is not found in cache, create a new entry
        node = init_node(key, value);
        cache_add(cache, node);
        map_put(cache->map, key, node);
        cache->size++;
    } else {
        // when the key already exists, update it with the current value
        node->value = value;
        cache_update(cache, node);
    }
}

// Function to free allocated memory for the node
void free_node(node_t* node) {
    free(node->value);
    free(node);
}

// Function to add key-value pair to cache and replace a random page if overflowing (Algo 1)
void cache_put_random(lru_cache* cache, void* key, void* value) {
    cache_put(cache, key, value);
    // Random replacement: remove a page chosen at random
    if (cache->size > cache->capacity) {
        int index = get_rand_int(0, cache->capacity);
        node_t* current = cache->sentinel->next;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        cache_remove(cache, current);
        map_remove(cache->map, current->key);
        free_node(current);
        cache->size--;
    }
}

// Function to add a key-value entry to cache and replace the least recently used page if overflowing (Algo 2)
void cache_put_LRU(lru_cache* cache, void* key, void* value) {
    cache_put(cache, key, value);
    // LRU method: remove the least recently used data
    if (cache->size > cache->capacity) {
        node_t* head = cache->sentinel->next;
        cache_remove(cache, head);
        map_remove(cache->map, head->key);
        free_node(head);
        cache->size--;
    }
}

// Function to display the content of cache in stdout
void display_cache(lru_cache* cache) {
    node_t* current = cache->sentinel->next;
    while (current->key != NULL) {
        printf("key: %s\n", (char*)current->key);
        printf("value: %p\n", current->value);
        current = current->next;
    }
}

// Function to free allocated memory for the cache
void free_cache(lru_cache* cache) {
    node_t* current = cache->sentinel->next;
    while (current->key != NULL) {
        node_t* temp = current;
        current = current->next;
        free_node(temp);
    }
    free(cache->sentinel);
    free_map(cache->map);
    free(cache);
}