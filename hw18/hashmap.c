#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

unsigned int hash(void* key) {
    return (unsigned int)key % HASH_MAP_SIZE;
}

HashMap* init_map() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    memset(map->buckets, 0, sizeof(Node*) * HASH_MAP_SIZE);
    return map;
}

void* map_get(HashMap* map, void* key) {
    unsigned int index = hash(key);
    Node* current = map->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void map_put(HashMap* map, void* key, void* value) {
    if (map_get(map, key) != NULL) {
        return;
    }
    unsigned int index = hash(key);
    Node* newNode = malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
    map->size++;
}

void map_remove(HashMap* map, void* key) {
    unsigned int index = hash(key);
    Node* current = map->buckets[index];
    Node* prev = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                // The node to be removed is the first node in the linked list
                map->buckets[index] = current->next;
            } else {
                // Update the previous node's next pointer to skip the current node
                prev->next = current->next;
            }
            map->size--;
            free(current); // Free the memory of the removed node
            return;
        }
        prev = current;
        current = current->next;
    }
}

void map_free(HashMap* map) {
    for (int i = 0; i < HASH_MAP_SIZE; ++i) {
        Node* current = map->buckets[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map);
}
