#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR 0.75
#define DICT_MAX_KEY_LENGTH 256

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

uint64_t fnv1a_64(const char* str) {
    uint64_t hval = 0xcbf29ce484222325ULL;
    unsigned char* s = (unsigned char*)str;

    while (*s) {
        hval ^= *s++;
        hval *= 0x100000001b3ULL;
    }

    return hval;
}

void resize(HashMap* map);
int put(HashMap* map, char* key, void* value);

HashMap* create_map() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map) {
        perror("malloc failed");
        return NULL;
    }
    map->entries = calloc(INITIAL_CAPACITY, sizeof(Entry));
    if (!map->entries) {
        perror("calloc failed");
        free(map);
        return NULL;
    }
    map->capacity = INITIAL_CAPACITY;
    map->size = 0;
    return map;
}

void resize(HashMap* map) {
    int old_capacity = map->capacity;
    Entry* old_entries = map->entries;

    map->capacity *= 2;
    map->entries = calloc(map->capacity, sizeof(Entry));
    if (!map->entries) {
        perror("calloc failed");
        // Handle error (e.g., exit or return an error code)
        exit(1);
    }
    map->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].key) {
            put(map, old_entries[i].key, old_entries[i].value);
        }
    }

    free(old_entries);
}

int put(HashMap* map, char* key, void* value) {
    if (map->size > (int)(map->capacity * LOAD_FACTOR)) {
        resize(map);
    }

    int h = hash(key);
    int index = h & (map->capacity - 1);

    while (map->entries[index].key != NULL) { // Check for existing key
        if (strcmp(map->entries[index].key, key) == 0) {
            // Key already exists; update value
            map->entries[index].value = value;
            return 0; // Indicate successful update
        }
        index = (index + 1) & (map->capacity - 1);
    }

    map->entries[index].key = strdup(key);
    if (!map->entries[index].key) {
        perror("strdup failed");
        return -1; // Indicate error
    }
    map->entries[index].value = value;
    map->entries[index].hash = h;
    map->size++;
    return 1; // Indicate successful insertion
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
    if (!map) return;
    for (int i = 0; i < map->capacity; i++) {
        if (map->entries[i].key) {
            free(map->entries[i].key);
        }
    }
    free(map->entries);
    free(map);
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