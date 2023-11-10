#ifndef HASHMAP_H
#define HASHMAP_H

#define HASH_MAP_SIZE 10000

typedef struct Node {
    char* key;
    void* value;
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[HASH_MAP_SIZE];
    size_t size;
} HashMap;

unsigned int hash(char* key);
HashMap* init_map();
void* map_get(HashMap* map, char* key);
void map_put(HashMap* map, char* key, void* value);
void map_remove(HashMap* map, char* key);
void free_map(HashMap* map);
void display_map(HashMap* map);

#endif