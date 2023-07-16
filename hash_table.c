#include <stdio.h>
#include <string.h>
#include "hash_table.h"

typedef struct entry {
    char* key; // Null terminated string
    void* object;
    struct entry *next;
} entry;

typedef struct _hash_table {
    uint32_t size;
    hashFunctrion* hash;
    entry** elements;
} hash_table;

hash_table* hash_table_create(uint32_t size, hashFunctrion* hf) {
    hash_table* ht = malloc(sizeof(*ht));
    ht->size = size;
    ht->hash = hf;
    ht->elements = calloc(sizeof(entry*), ht->size); // Zero out the memory
    return ht;
}

void hash_table_destroy(hash_table* ht) {

    free(ht->elements);
    free(ht);
}

void hash_table_print(hash_table* ht) {
    printf("Start table");
    for (uint32_t i = 0; i < ht->size; ++i) {
        if (ht->elements[i] != NULL) {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t\n", i);
            entry* temp = ht->elements[i];
            while (temp != NULL) {
                printf("\"%s\"(%p) - ", temp->key, temp->object);
                temp = temp->next;
            }
            printf("\n");
        }
    }
    printf("End table");
}

static size_t hash_table_index(hash_table* ht, const char* key) {
    size_t result = (ht->hash(key, strlen(key)) % ht->size);
    return result;
}

bool hash_table_insert(hash_table* ht, const char* key, void* value) {
    // No nulls allowed
    if (key == NULL || value == NULL)
        return false;
    
    // Key already in the table
    size_t index = hash_table_index(ht, key);
    if (hash_table_lookup(ht, key) != NULL)
        return false;

    entry* e = malloc(sizeof(*e));
    e->object = value;
    e->key = malloc(strlen(key)+1);
    strcpy(e->key, key);

    e->next = ht->elements[index];
    ht->elements[index] = e;

    return true;
}

void* hash_table_lookup(hash_table* ht, const char* key) {
    // No nulls allowed
    if (key == NULL || ht == NULL)
        return false;

    size_t index = hash_table_index(ht, key);
    entry* temp = ht->elements[index];
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        temp = temp->next;
    }

    if (temp == NULL) 
        return NULL;

    return temp->object;
}

void* hash_table_delete(hash_table* ht, const char* key) {
    // No nulls allowed
    if (key == NULL || ht == NULL)
        return false;

    size_t index = hash_table_index(ht, key);
    entry* temp = ht->elements[index];
    entry* prev = NULL;
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) 
        return NULL;

    if (prev == NULL) { // Delete the head of the list
        ht->elements[index] = temp->next;
    } else { // Deleting from the middle
        prev->next = temp->next;
    }
    
    void* result = temp->object;
    free(temp);

    return temp->object; // The user takes care of deleting the object
}
