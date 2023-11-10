#include <stdio.h>
#include <stdlib.h>
#include "LRUcache.h"

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

void* cache_get(lru_cache* cache, void* key) {
    node_t* node = (node_t*)map_get(cache->map, key);
    if (node == NULL) {
        return NULL;
    }
    cache_update(cache, node);
    return node->value;
}

void cache_put(lru_cache* cache, void* key, void* value) {
    node_t* node = map_get(cache->map, key);
    if (node == NULL) {
        // when the key is not found in cache, create a new entry
        node = init_node(key, value);
        cache_add(cache, node);
        map_put(cache->map, key, node);
    } else {
        // when the key already exists, update it with the current value
        node->value = value;
        cache_update(cache, node);
    }
    // LRU method: remove the least recently used data
    if (cache->map->size > cache->capacity) {
        node_t* head = cache->sentinel->next;
        cache_remove(cache, head);
        map_remove(cache->map, head->key);
    }
}

void display_cache(lru_cache* cache) {
    node_t* current = cache->sentinel->next;
    while (current->key != NULL) {
        printf("key: %s\n", (char*)current->key);
        printf("value: %p\n", current->value);
        current = current->next;
    }
}

void free_cache(lru_cache* cache) {
    node_t* current = cache->sentinel;
    while (current != NULL) {
        node_t* temp = current;
        current = current->next;
        free(temp);
    }
    free_map(cache->map);
    free(cache);
}