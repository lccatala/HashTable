#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint64_t hashFunctrion (const char*, size_t);
typedef struct _hash_table hash_table;

hash_table* hash_table_create(uint32_t size, hashFunctrion* hf);
void hash_table_destroy(hash_table* ht);
void hash_table_print(hash_table* ht);
bool hash_table_insert(hash_table* ht, const char* key, void* value);
void* hash_table_lookup(hash_table* ht, const char* key);
void* hash_table_delete(hash_table* ht, const char* key);

#endif
