#ifndef HASHTABLE_GUARD
#define HASHTABLE_GUARD

typedef struct HashTableElement {
    char *key;
    void *value;
    struct HashTableElement *next;
} HashTableElement;

typedef struct HashTable {
    HashTableElement **array;
    int size;
} HashTable;

HashTable *make_hashtable(int size);

void free_hashtable(HashTable *table);

void insert_to_hashtable(HashTable *table, char *key, unsigned long key_size, void *value);

void *get_from_hashtable(HashTable *table, char *key, unsigned long key_size);

#endif