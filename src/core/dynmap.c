#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "dynarray.h"
#include "seawitch.h"

#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR 0.75
#define DICT_MAX_KEY_LENGTH 256

typedef struct {
    DynArray* keys; // keys are char[256]
    DynArray* values;   // values are generic
    DynArray* available;   // array of bools to track if slot is available. Used for tombstones
    
    Types value_type;
    size_t value_size;
    size_t len;
    size_t capacity;
} DynDict;

// FNV-1a hash function
size_t hash(const char* str) {
    size_t hval = 0xcbf29ce484222325ULL;
    unsigned char* s = (unsigned char*)str;

    while (*s) {
        hval ^= *s++;
        hval *= 0x100000001b3ULL;
    }

    return hval;
}

// Create a new dictionary
DynDict* dyndict_create(Types value_type, size_t value_size, size_t initial_capacity) {
    DynDict* dict = calloc(1, sizeof(DynDict));
    if (!dict) return NULL;

    dict->keys = dynarray_create(REF_BYTE, DICT_MAX_KEY_LENGTH, initial_capacity);
    if (!dict->keys) return print_error_return_null(__FILE__, __LINE__);

    dict->values = dynarray_create(value_type, value_size, initial_capacity);
    if (!dict->values) return print_error_return_null(__FILE__, __LINE__);

    dict->available = dynarray_create(BOOL, sizeof(Bool), initial_capacity);
    if (!dict->available) return print_error_return_null(__FILE__, __LINE__);

    dict->value_type = value_type;
    dict->value_size = value_size;
    dict->len = 0;
    dict->capacity = initial_capacity;

    // Initialize available array to true
    for (size_t i = 0; i < initial_capacity; i++) {
        dynarray_push(dict->available, &(Bool){true});
    }

    return dict;
}

// Internal function to resize the dictionary. Use the load factor to determine when to resize
// Returns true if the resize was successful or not needed. False otherwise
Bool dyndict_resize(DynDict* dict) {
    if (!dict) return print_error_return_false(__FILE__, __LINE__);
    if (((double)dict->len / dict->capacity) < DICT_LOAD_FACTOR) return true; //skip. Hash table is not overloaded

    size_t old_capacity = dict->capacity;
    size_t new_capacity = dict->capacity * 2;
    if (new_capacity < dict->capacity)  return print_error_return_false(__FILE__, __LINE__); // Overflow

    // Resize the arrays and rehash the keys
    // Instead of realloc, we create new arrays and copy the data over as we have to rehash anyway
    DynArray* new_keys = dynarray_create(REF_BYTE, DICT_MAX_KEY_LENGTH, new_capacity);
    DynArray* new_values = dynarray_create(dict->value_type, dict->value_size, new_capacity);
    DynArray* new_available = dynarray_create(BOOL, sizeof(Bool), new_capacity);

    // mark the new slots as available
    for (size_t i = old_capacity; i < new_capacity; i++) {
        dynarray_push(new_available, &(Bool){true});
    }

    // rehash and reindex the keys
    for (size_t i = 0; i < old_capacity; i++) {
        if (!*(Bool*)dynarray_get(dict->available, i, &(Bool){true})) {
            char key[DICT_MAX_KEY_LENGTH];
            dynarray_get(dict->keys, i, key);
            void* value = calloc(1, dict->value_size);
            dynarray_get(dict->values, i, value);

            size_t hval = hash(key) % new_capacity;
            size_t original_hval = hval;
            size_t j = 0;
            while (!*(Bool*)dynarray_get(new_available, hval, &(Bool){true})) {
                hval = (original_hval + ++j) % new_capacity;
            }

            dynarray_set(new_keys, hval, key);
            dynarray_set(new_values, hval, value);
            dynarray_set(new_available, hval, &(Bool){false});
        }
    }

    dict->keys = new_keys;
    dict->values = new_values;
    dict->available = new_available;
    dict->capacity = new_capacity;

    return true;
}

// Push a key-value pair to the dictionary
Bool dyndict_push(DynDict* dict, char* key, void* value) {
    if (!dict || !key || !value) return print_error_return_false(__FILE__, __LINE__);
    if (!dyndict_resize(dict)) return false;

    size_t hval = hash(key) % dict->capacity;
    size_t original_hval = hval;
    size_t i = 0;
    while (*(Bool*)dynarray_get(dict->available, hval, &(Bool){0})) {
        if (i >= dict->capacity) break; // Prevent infinite loop

        char k[DICT_MAX_KEY_LENGTH];
        dynarray_get(dict->keys, hval, k);
        if (strcmp(k, key) == 0) return false; // Key already exists
        hval = (original_hval + ++i) % dict->capacity;
    }

    char key_copy[DICT_MAX_KEY_LENGTH];
    strncpy(key_copy, key, DICT_MAX_KEY_LENGTH - 1);
    key_copy[DICT_MAX_KEY_LENGTH - 1] = '\0';

    if (
        dynarray_set(dict->keys, hval, key_copy) &&
        dynarray_set(dict->values, hval, value) &&
        dynarray_set(dict->available, hval, &(Bool){false})
    ) {
        dict->len++;
        return true;
    }

    return false;
}

void* dyndict_get(DynDict* dict, char* key, void* out) {
    if (!dict || !key || !out) return print_error_return_null(__FILE__, __LINE__);

    size_t hval = hash(key) % dict->capacity;
    size_t original_hval = hval;
    size_t i = 0;
    while (*(Bool*)dynarray_get(dict->available, hval, &(Bool){0}) == false) {
        if (i >= dict->capacity) break; // Prevent infinite loop

        char k[DICT_MAX_KEY_LENGTH];
        dynarray_get(dict->keys, hval, k);
        if (strcmp(k, key) == 0) {
            return dynarray_get(dict->values, hval, out);
        }
        hval = (original_hval + ++i) % dict->capacity;
    }

    return NULL;
}

Bool dyndict_set(DynDict* dict, char* key, void* value) {
    if (!dict || !key || !value) return print_error_return_false(__FILE__, __LINE__);

    size_t hval = hash(key) % dict->capacity;
    size_t original_hval = hval;
    size_t i = 0;
    while (*(Bool*)dynarray_get(dict->available, hval, &(Bool){0})) {
        if (i >= dict->capacity) break; // Prevent infinite loop

        char k[DICT_MAX_KEY_LENGTH];
        dynarray_get(dict->keys, hval, k);
        if (strcmp(k, key) == 0) {
            return dynarray_set(dict->values, hval, value);
        }
        hval = (original_hval + ++i) % dict->capacity;
    }

    return dyndict_push(dict, key, value);
}

void* dyndict_delete(DynDict* dict, char* key, void* out) {
    if (!dict || !key || !out) return print_error_return_null(__FILE__, __LINE__);

    size_t hval = hash(key) % dict->capacity;
    size_t original_hval = hval;
    size_t i = 0;

    while (*(Bool*)dynarray_get(dict->available, hval, &(Bool){false})) {
        if (i >= dict->capacity) break; // Prevent infinite loop

        char k[DICT_MAX_KEY_LENGTH];
        dynarray_get(dict->keys, hval, k);

        if (strcmp(k, key) == 0) {
            // Mark the slot as available (tombstoned! Mah gahd that value had a family!)
            Bool tombstone = true;
            dynarray_set(dict->available, hval, &tombstone);
            dict->len--; // Decrement the number of items in the hashmap

            //Retrieve the value
            return dynarray_get(dict->values, hval, out);
        }
        hval = (original_hval + ++i) % dict->capacity;
    }

    return NULL; // Key not found
}

// check if key exists in the dictionary
Bool dyndict_contains(DynDict* dict, char* key) {
    if (!dict || !key) return false;
    size_t hval = hash(key) % dict->capacity;
    size_t i = 0;
    while (!*(Bool*)dynarray_get(dict->available, hval, &(Bool){true})) {
        if (i >= dict->capacity) break; // Prevent infinite loop

        char k[DICT_MAX_KEY_LENGTH];
        dynarray_get(dict->keys, hval, k);
        if (strcmp(k, key) == 0) return true;
        hval = (hval + ++i) % dict->capacity;
    }
    return false;
}

// Find value in the dictionary. take a predicate function to compare the values
char* dyndict_find(DynDict* dict, void* out, Bool (*predicate)(void*, void*)) {
    if (!dict || !predicate) return print_error_return_null(__FILE__, __LINE__);

    for (size_t i = 0; i < dict->capacity; i++) {
        if (!*(Bool*)dynarray_get(dict->available, i, &(Bool){true})) {
            char key[DICT_MAX_KEY_LENGTH];
            dynarray_get(dict->keys, i, key);
            void* value = dynarray_get(dict->values, i, NULL);
            if (predicate(value, out)) return key;
        }
    }

    return NULL;
}

// on each - iterate over each key-value pair with a function
void dyndict_for_each(DynDict* dict, void (*callback)(size_t, char*, void*)) {
    for (size_t i = 0; i < dict->capacity; i++) {
        if (!*(Bool*)dynarray_get(dict->available, i, &(Bool){true})) {
            char key[DICT_MAX_KEY_LENGTH];
            dynarray_get(dict->keys, i, key);
            void* value = dynarray_get(dict->values, i, NULL);
            callback(i, key, value);
        }
    }
}

// Filter - filter the dictionary based on a predicate and return a new dictionary
DynDict* dyndict_filter(DynDict* dict, Bool (*predicate)(void*, void*)) {
    if (!dict || !predicate) return print_error_return_null(__FILE__, __LINE__);

    DynDict* result = dyndict_create(dict->value_type, dict->value_size, dict->capacity);
    if (!result) return NULL;

    for (size_t i = 0; i < dict->capacity; i++) {
        if (!*(Bool*)dynarray_get(dict->available, i, &(Bool){true})) {
            char key[DICT_MAX_KEY_LENGTH];
            dynarray_get(dict->keys, i, key);
            void* value = dynarray_get(dict->values, i, NULL);
            if (predicate(value, NULL)) {
                dyndict_push(result, key, value);
            }
        }
    }

    return result;
}

// Order - order the dictionary based on the order of entry
// sort - sort the dictionary based on the keys & values