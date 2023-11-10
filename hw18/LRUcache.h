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

node_t* init_node(void* key, void* value);
lru_cache* init_cache(size_t capacity);
void cache_remove(lru_cache* cache, node_t* node);
void cache_add(lru_cache* cache, node_t* node);
void cache_update(lru_cache* cache, node_t* node);
void* cache_get(lru_cache* cache, void* key);
void cache_put(lru_cache* cache, void* key, void* value);

#endif