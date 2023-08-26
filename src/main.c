#include <stdio.h>
#include "otp/hashtable.h"

int main() {
    
    HashTable *table = make_hashtable(10);
    char key[] = "Some key\n";
    int value = 10;
    insert_to_hashtable(table, key, sizeof(key), &value);
    char key2[] = "Some key\n";
    int value2 = 21;
    insert_to_hashtable(table, key2, sizeof(key2), &value2);
    int retrieved = *((int*)get_from_hashtable(table, key, sizeof(key)));
    printf("%d\n", retrieved);

    free_hashtable(table);

    return 0;
}