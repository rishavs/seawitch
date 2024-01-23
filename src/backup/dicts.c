#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.7
#define INITIAL_CAPACITY 16

typedef struct KeyValuePair {
    char* key;
    void* value;
    struct KeyValuePair* next;
} KeyValuePair;

typedef struct Dictionary {
    size_t size;
    size_t capacity;
    KeyValuePair** table;
} Dictionary;

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

Dictionary* create_dictionary() {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL; // malloc failed
    }
    dict->size = 0;
    dict->capacity = INITIAL_CAPACITY;
    dict->table = (KeyValuePair**)calloc(INITIAL_CAPACITY, sizeof(KeyValuePair*));
    if (dict->table == NULL) {
        free(dict);
        return NULL; // calloc failed
    }
    return dict;
}

void destroy_dictionary(Dictionary* dict) {
    for (size_t i = 0; i < dict->capacity; i++) {
        KeyValuePair* current = dict->table[i];
        while (current != NULL) {
            KeyValuePair* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(dict->table);
    free(dict);
}

void insert(Dictionary* dict, const char* key, void* value) {
    if ((double)dict->size / dict->capacity >= MAX_LOAD_FACTOR) {
        size_t new_capacity = dict->capacity * 2;
        KeyValuePair** new_table = (KeyValuePair**)calloc(new_capacity, sizeof(KeyValuePair*));
        if (new_table == NULL) {
            return; // Memory allocation failed
        }
        
        for (size_t i = 0; i < dict->capacity; i++) {
            KeyValuePair* current = dict->table[i];
            while (current != NULL) {
                KeyValuePair* next = current->next;
                size_t index = hash(current->key) % new_capacity;
                current->next = new_table[index];
                new_table[index] = current;
                current = next;
            }
        }
        
        free(dict->table);
        dict->table = new_table;
        dict->capacity = new_capacity;
    }
    
    KeyValuePair* new_pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    if (new_pair == NULL) {
        return; // Memory allocation failed
    }
    new_pair->key = strdup(key);
    new_pair->value = value;
    new_pair->next = NULL;
    
    size_t index = hash(key) % dict->capacity;
    new_pair->next = dict->table[index];
    dict->table[index] = new_pair;
    dict->size++;
}

void* get(const Dictionary* dict, const char* key) {
    size_t index = hash(key) % dict->capacity;
    KeyValuePair* current = dict->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void set(Dictionary* dict, const char* key, void* value) {
    size_t index = hash(key) % dict->capacity;
    KeyValuePair* current = dict->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value; // Update the value
            return;
        }
        current = current->next;
    }
    insert(dict, key, value);
}

int main() {
    Dictionary* dict = create_dictionary();

    insert(dict, "name", "John");
    insert(dict, "age", (void*)30);
    insert(dict, "city", "New York");

    printf("Name: %s\n", (char*)get(dict, "name"));
    printf("Age: %d\n", (int)(intptr_t)get(dict, "age"));
    printf("City: %s\n", (char*)get(dict, "city"));

    set(dict, "age", (void*)31);
    printf("Updated Age: %d\n", (int)(intptr_t)get(dict, "age"));

    destroy_dictionary(dict);

    return 0;
}