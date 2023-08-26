#include "hashtable.h"
#include <stdlib.h>

typedef unsigned long uint32;
typedef unsigned long long uint64;

static uint64 calc_hash(char *str, uint32 size) {
    uint64 hash = 0;
    for (int i = 0; i < size; ++i) {
        hash += (i+1) * str[i];
    }
    return hash;
}

static int calc_index(char *key, uint32 key_size, int table_size) {
    return calc_hash(key, key_size) % table_size;
}

static char compare_keys(char *key1, char *key2) {
    int ofs = 0;
    while (*(key1+ofs) != '\0' || *(key2+ofs) != '\0') {
        if (*(key1+ofs) != *(key2+ofs))
            return 0;
        ++ofs;
    }
    return *(key1+ofs) == '\0' && *(key2+ofs) == '\0';
}

HashTable *make_hashtable(int size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->array = malloc(sizeof(HashTableElement*) * size);
    return table;
}

void free_hashtable(HashTable *table) {
    for (int i = 0; i < table->size; ++i) {
        HashTableElement *curr = table->array[i], *next;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(table->array);
    free(table);
}

static HashTableElement *make_hashtable_element(char *key, void *value) {
    HashTableElement *element = malloc(sizeof(HashTableElement));
    element->key = key;
    element->value = value;
    element->next = NULL;
    return element;
}

void insert_to_hashtable(HashTable *table, char *key, uint32 key_size, void *value) {
    int index = calc_index(key, key_size, table->size);
    HashTableElement **curr = &table->array[index];
    while (*curr != NULL) {
        if (compare_keys(key, (*curr)->key)) {
            (*curr)->value = value;
            return;
        }
        curr = &(*curr)->next;
    }
    *curr = make_hashtable_element(key, value);    
}

void *get_from_hashtable(HashTable *table, char *key, uint32 key_size) {
    int index = calc_index(key, key_size, table->size);
    HashTableElement *curr = table->array[index];
    while (curr != NULL) {
        if (compare_keys(key, curr->key))
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}