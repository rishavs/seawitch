#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75

typedef struct Entry {
    char* key;
    void* value;
    int hash;
} Entry;

typedef struct HashMap {
    Entry* entries;
    int capacity;
    int size;
} HashMap;

int hash(char* key) {
    int hash = 0;
    while (*key) {
        hash = (hash << 5) - hash + *key++;
    }
    return hash;
}

void resize (HashMap*);
void put(HashMap*, char*, void*);

HashMap* create_map() {
    HashMap* map = malloc(sizeof(HashMap));
    map->entries = calloc(INITIAL_CAPACITY, sizeof(Entry));
    map->capacity = INITIAL_CAPACITY;
    map->size = 0;
    return map;
}

void resize(HashMap* map) {
    int old_capacity = map->capacity;
    Entry* old_entries = map->entries;

    map->capacity *= 2;
    map->entries = calloc(map->capacity, sizeof(Entry));
    map->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].key) {
            put(map, old_entries[i].key, old_entries[i].value);
        }
    }

    free(old_entries);
}

void put(HashMap* map, char* key, void* value) {
    if (map->size > map->capacity * LOAD_FACTOR) {
        resize(map);
    }

    int index = hash(key) & (map->capacity - 1);
    int dist = 0;

    while (map->entries[index].key && dist > index - (map->entries[index].hash & (map->capacity - 1))) {
        index = (index + 1) & (map->capacity - 1);
        dist++;
    }

    Entry new_entry = { .key = strdup(key), .value = value, .hash = hash(key) };
    while (map->entries[index].key) {
        Entry temp = map->entries[index];
        map->entries[index] = new_entry;
        new_entry = temp;
        index = (index + 1) & (map->capacity - 1);
    }

    map->entries[index] = new_entry;
    map->size++;
}

void* get(HashMap* map, char* key) {
    int index = hash(key) & (map->capacity - 1);

    while (map->entries[index].key) {
        if (strcmp(map->entries[index].key, key) == 0) {
            return map->entries[index].value;
        }
        index = (index + 1) & (map->capacity - 1);
    }

    return NULL;
}

void delete_map(HashMap* map) {
    for (int i = 0; i < map->capacity; i++) {
        if (map->entries[i].key) {
            free(map->entries[i].key);
        }
    }
    free(map->entries);
    free(map);
}

int main() {
    // Create a new hashmap
    HashMap* map = create_map();

    // Create some values to put in the hashmap
    int value1 = 123;
    int value2 = 456;
    int value3 = 789;

    // Put the values in the hashmap
    put(map, "key1", &value1);
    put(map, "key2", &value2);
    put(map, "key3", &value3);

    // Get the values from the hashmap
    int* retrieved1 = (int*)get(map, "key1");
    int* retrieved2 = (int*)get(map, "key2");
    int* retrieved3 = (int*)get(map, "key3");

    // Print the retrieved values
    printf("Retrieved values: %d, %d, %d\n", *retrieved1, *retrieved2, *retrieved3);

    // Delete the hashmap
    delete_map(map);

    return 0;
}


// int main() {
//     // Create a new hashmap
//     HashMap* map = create_map();

//     // Create some values to put in the hashmap
//     int value1 = 123;
//     int value2 = 456;
//     int value3 = 789;

//     // Put the values in the hashmap
//     put(map, "key1", &value1);
//     put(map, "key2", &value2);
//     put(map, "key3", &value3);

//     // Get the values from the hashmap
//     int* retrieved1 = (int*)get(map, "key1");
//     int* retrieved2 = (int*)get(map, "key2");
//     int* retrieved3 = (int*)get(map, "key3");

//     // Print the retrieved values
//     printf("Retrieved values: %d, %d, %d\n", *retrieved1, *retrieved2, *retrieved3);

//     // Delete the hashmap
//     delete_map(map);

//     return 0;
// }