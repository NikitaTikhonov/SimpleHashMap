#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100000
#define TRUE 1
#define FALSE 0
#define DELIMETER printf("-------------------------------------\n")

typedef struct entry_t
{
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

typedef struct
{
    entry_t **entries;
} ht_t;

//hash function for our HashMap
unsigned int hash(const char *key)
{
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    // do multiplication several times
   while (i < key_len) {
        value = value * 37 + key[i];
        ++i;
    }

    // make sure value is 0 <= value <= TABLE_SIZE
    value = value % TABLE_SIZE;

    return value;
}

entry_t *ht_pair(const char *key, const char *value)
{
    // allocate the entry
    entry_t *entry = malloc(sizeof(entry_t) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    // copy key and value in place
    strcpy(entry->key, key);
    strcpy(entry->value, value);

    entry->next = NULL;

    return entry;
}

ht_t *ht_create(void)
{
    // allocate memory for table
    ht_t *hashtable = malloc(sizeof(ht_t) * 1);

    // allocate table etries
    hashtable->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

    // set each node to null
    int i = 0;
      while (i < TABLE_SIZE) {
        hashtable->entries[i] = NULL;
        ++i;
    }

    return hashtable;
}

void ht_set(ht_t *hashtable, const char *key, const char *value)
{
    unsigned int slot = hash(key);

    // try to find existing entry
    entry_t *entry = hashtable->entries[slot];

    // if no entry means slot empty, insert immediately
    if (entry == NULL)
    {
        hashtable->entries[slot] = ht_pair(key, value);
        return;
    }

    entry_t *prev;

    // go through each entry untill either the end is reached or a matchis string is found
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        }

        //go to next
        prev = entry;
        entry = prev->next;
    }

    // end of the chain reached withouth a match, add new
    prev->next = ht_pair(key, value);
}

char *ht_get(ht_t *hashtable, const char *key)
{
    unsigned int slot = hash(key);

    entry_t *entry = hashtable->entries[slot];

    //no slot means no entry
    if (entry == NULL)
    {
        return NULL;
    }

    //go through each entry in the slot, which could be a single thing
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }

        entry = entry->next;
    }

    return NULL;
}

void ht_dump(ht_t *hashtable)
{
    int i = 0;
    while (i < TABLE_SIZE)
    {
        entry_t *entry = hashtable->entries[i];

        if (entry == NULL)
        {
            ++i;
            continue;
        }

        printf("slot[%4d]: ", i);

        while (TRUE)
        {
            printf("%s=%s ", entry->key, entry->value);

            if (entry->next == NULL)
            {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
        ++i;
    }
}

void ht_del(ht_t *hashtable, const char *key) {
    unsigned int bucket = hash(key);

    // try to find a valid bucket
    entry_t *entry = hashtable->entries[bucket];

    // no bucket means no entry
    if (entry == NULL) {
        return;
    }

    entry_t *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // first item and no next entry
            if (entry->next == NULL && idx == 0) {
                hashtable->entries[bucket] = NULL;
            }

            // first item with a next entry
            if (entry->next != NULL && idx == 0) {
                hashtable->entries[bucket] = entry->next;
            }

            // last item
            if (entry->next == NULL && idx != 0) {
                prev->next = NULL;
            }

            // middle item
            if (entry->next != NULL && idx != 0) {
                prev->next = entry->next;
            }

            // free the deleted entry
            free(entry->key);
            free(entry->value);
            free(entry);

            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;

        ++idx;
    }
}


int main()
{
    ht_t *ht = ht_create();

    ht_set(ht, "test1", "val1");
    ht_set(ht, "test2", "val2");
    ht_set(ht, "test3", "val3");
    ht_set(ht, "test4", "val4");
    ht_set(ht, "test5", "val5");
    ht_set(ht, "test6", "val6");
    ht_set(ht, "test6", "val7");

    ht_dump(ht);


    DELIMETER;
    ht_set(ht, "test6", "anotherVal");
    ht_dump(ht);

    DELIMETER;
    ht_del(ht, "test6");
    ht_dump(ht);

    return 1;
}