#ifndef LRUCACHE_H
#define LRUCACHE_H

#include "hashmap.h"

typedef struct node_t {
    void* key;
    void* value;
    struct node_t* prev;
    struct node_t* next;
} node_t;

typedef struct lru_cache {
    node_t* sentinel;
    size_t size;
    size_t capacity;
    HashMap* map;
} lru_cache;

lru_cache* init_cache(size_t capacity);
void* cache_get(lru_cache* cache, void* key);
void cache_put_random(lru_cache* cache, void* key, void* value);
void cache_put_LRU(lru_cache* cache, void* key, void* value);
void free_cache(lru_cache* cache);
void display_cache(lru_cache* cache);

#endif