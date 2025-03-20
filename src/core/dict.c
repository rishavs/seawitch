#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h> // For SIZE_MAX

#include "sw_bootstrap.h" // Assuming this provides strict_calloc

#define DICT_KEY_SIZE 64
#define DICT_INITIAL_CAPACITY 16
#define DICT_LOAD_FACTOR 0.75

typedef char Byte;

typedef struct {
    char* message;                // Error message. Heap
    char* name;                   // Error header. Optional. Heap
    char* details;                // Optional. cause. hint. etc. Heap
    
    size_t pos;                  // Error position provided by user. Optional
    size_t line;                 // Error line number provided by user. Optional
    size_t column;               // Error column number provided by user. Optional
    char* filepath;               // Error file path provided by user. Optional. Heap

    size_t raised_on_line;       // Internal compiler path for error.
    char* raised_in_file;         // Internal compiler path for error. Heap
} CError;

typedef enum {
    EMPTY,
    OCCUPIED,
    DELETED
} EntryState;

typedef enum {
    // Value types
    STRING,
    INTEGER,
    FLOAT,
    BOOL,
    PTR
} Entry_kind;

typedef struct Entry {
    char key[DICT_KEY_SIZE];
    union {
        char*   str_value; // For STRING
        int64_t int64_value; // For INTEGER
        double  float64_value; // For FLOAT
        bool    bool_value; // For BOOL
        void*   ptr_value; // For PTR
    } value;
    EntryState state;
} Entry;

typedef struct Dict {
    size_t capacity;
    size_t len;
    Entry* entries;
    size_t entry_size;
    Entry_kind entry_kind;
} Dict;


// APIs
Dict*   dict_create(size_t initial_capacity, size_t entry_size, Entry_kind entry_kind);
size_t* dict_find(const Dict* dict, const char* key);
CError* dict_get(const Dict* dict, const char* key, void* out);
CError* dict_set(Dict* dict, const char* key, void* value);
CError* dict_remove(Dict* dict, const char* key);

// FNV-1a hash function (remains the same)
uint64_t fnv1a_hash(const char* str) {
    uint64_t hash = 14695981039346656037ULL;
    for (const char* p = str; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= 1099511628211ULL;
    }
    return hash;
}

// API: create
Dict* dict_create(size_t initial_capacity, size_t entry_size, Entry_kind entry_kind) {
    Dict* dict = strict_calloc(1, sizeof(Dict), __FILE__, __LINE__);
    dict->entry_size = entry_size;
    dict->entry_kind = entry_kind;
    dict->capacity = initial_capacity ? initial_capacity : DICT_INITIAL_CAPACITY;
    dict->len = 0;
    dict->entries = strict_calloc(dict->capacity, sizeof(Entry), __FILE__, __LINE__);
    for (size_t i = 0; i < dict->capacity; i++) {
        dict->entries[i].state = EMPTY;
    }
    return dict;
}

// Helper function to find an empty or deleted slot for a given key
static size_t find_available_slot(const Dict* dict, const char* key) {
    uint64_t hash = fnv1a_hash(key);
    size_t capacity = dict->capacity;
    size_t h = (size_t)(hash % capacity);
    size_t i = 0;

    while (i < capacity) {
        size_t index = (h + i * i) % capacity;
        if (dict->entries[index].state == EMPTY || dict->entries[index].state == DELETED) {
            return index;
        }
        i++;
    }
    return SIZE_MAX; // Should not happen if load factor is managed
}

static bool dict_resize(Dict* dict, size_t new_capacity) {
    Entry* old_entries = dict->entries;
    size_t old_capacity = dict->capacity;

    dict->entries = strict_calloc(new_capacity, sizeof(Entry), __FILE__, __LINE__);
    if (!dict->entries) return false;

    dict->capacity = new_capacity;
    dict->len = 0;
    for (size_t i = 0; i < new_capacity; i++) {
        dict->entries[i].state = EMPTY;
    }

    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == OCCUPIED) {
            dict_set(dict, old_entries[i].key, old_entries[i].value);
        }
    }

    free(old_entries);
    return true;
}

// API: find (return index if found, SIZE_MAX otherwise)
size_t dict_find(const Dict* dict, const char* key) {
    uint64_t hash = fnv1a_hash(key);
    size_t capacity = dict->capacity;
    size_t h = (size_t)(hash % capacity);
    size_t i = 0;

    while (i < capacity) {
        size_t index = (h + i * i) % capacity;
        if (dict->entries[index].state == OCCUPIED && strcmp(dict->entries[index].key, key) == 0) {
            return index;
        }
        if (dict->entries[index].state == EMPTY) {
            return SIZE_MAX; // Key not found
        }
        i++;
    }
    return SIZE_MAX; // Should not happen if load factor is managed
}

// API: get
CError* dict_get(const Dict* dict, const char* key, void* out) {
    size_t index = dict_find(dict, key);
    if (index != SIZE_MAX) {
        return dict->entries[index].value;
    }
    return snitch("Key not found", __FILE__, __LINE__); // Error handling
}

// API: set (insert or update)
bool dict_set(Dict* dict, const char* key, void* value) {
    if (!dict || !key) return false;

    if ((double)(dict->len + 1) / dict->capacity > DICT_LOAD_FACTOR) {
        if (!dict_resize(dict, dict->capacity * 2)) {
            return false;
        }
    }

    size_t existing_index = dict_find(dict, key);
    if (existing_index != SIZE_MAX) {
        dict->entries[existing_index].value = value;
        dict->entries[existing_index].state = OCCUPIED;
        return true;
    } else {
        size_t index_to_insert = find_available_slot(dict, key);
        if (index_to_insert != SIZE_MAX) {
            strcpy(dict->entries[index_to_insert].key, key);
            dict->entries[index_to_insert].value = value;
            dict->entries[index_to_insert].state = OCCUPIED;
            dict->len++;
            return true;
        }
        return false; // Should not happen if resize is correct
    }
}

// API: remove
void dict_remove(Dict* dict, const char* key) {
    size_t index = dict_find(dict, key);
    if (index != SIZE_MAX) {
        dict->entries[index].state = DELETED;
        dict->len--;
    }
}

// Helper function to destroy the dictionary
void dict_destroy(Dict* dict) {
    if (dict) {
        free(dict->entries);
        free(dict);
    }
}

