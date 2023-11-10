#ifndef HASHMAP_H
#define HASHMAP_H

#define HASH_MAP_SIZE 10000

typedef struct Node {
    void* key;
    void* value;
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[HASH_MAP_SIZE];
    size_t size;
} HashMap;

unsigned int hash(void* key);
HashMap* init_map();
void* map_get(HashMap* map, void* key);
void map_put(HashMap* map, void* key, void* value);
void map_remove(HashMap* map, void* key);
void map_free(HashMap* map);

#endif