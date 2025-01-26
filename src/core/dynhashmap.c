#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "dynarray.h"
#include "dynhashmap.h"
#include "seawitch.h"

#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR 0.75
#define DICT_MAX_KEY_LENGTH 256

// FNV-1a hash function
Int64 hash(const char* str) {
    Int64 hval = 0xcbf29ce484222325LL;
    unsigned char* s = (unsigned char*)str;

    while (*s) {
        hval ^= *s++;
        hval *= 0x100000001b3LL;
    }

    return hval;
}

// Create a new dictionary
// Note: the dict will only grow, as required, and not shrink.
DynHashmap* dynhashmap_create(Types value_type, Int64 value_size, Int64 initial_capacity) {
    DynHashmap* hmap = calloc(1, sizeof(DynHashmap));
    if (hmap ==  NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    Int64 optimal_capacity = initial_capacity > 0 ? initial_capacity * 2 : DICT_INITIAL_CAPACITY; 

    Error err;

    hmap->keys      = *dynarray_create(FXSTRING, FIXED_STRING_SIZE, optimal_capacity);
    hmap->values    = *dynarray_create(value_type, value_size, optimal_capacity);
    hmap->available = *dynarray_create(BOOL, sizeof(Bool), optimal_capacity);

    hmap->value_type = value_type;
    hmap->value_size = value_size;
    hmap->len = 0;
    hmap->capacity = optimal_capacity;

    // Initialize available array to true
    for (Int64 i = 0; i < optimal_capacity; i++) {
        Bool available = true;
        err = dynarray_push(&hmap->available, &available, sizeof(Bool));
        if (!err.ok) fatal(snitch(err.message.data, __LINE__, __FILE__));
    }

    return hmap;
}

Error dynhashmap_resize(DynHashmap* hashmap) {
    if(hashmap == NULL || hashmap->keys.data == NULL || hashmap->values.data == NULL || hashmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);

    // Check if the load factor is less than the threshold
    if ((float)hashmap->len / hashmap->capacity < DICT_LOAD_FACTOR) return (Error){.ok = true};
    Error err;

    Int64 new_capacity = hashmap->capacity * 2;
    DynHashmap* new_hmap = dynhashmap_create(hashmap->value_type, hashmap->value_size, new_capacity);

    Bool avl;
    // Now iterate over the old hashmap, get the value and insert it into the new hashmap
    for (Int64 i = 0; i < hashmap->capacity; i++) {
        err = dynarray_get(&hashmap->available, i, &avl);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // Skip if the slot is available
        if (avl) continue;

        // if the slot is in use, then insert it into the new hashmap
        FxString key;
        err = dynarray_get(&hashmap->keys, i, &key);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        void* value;
        err = dynarray_get(&hashmap->values, i, &value);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
        
        // Insert the key and value into the new hashmap
        err = dynhashmap_set(new_hmap, key.data, value); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    }

    return (Error){.ok = true};
}


// Set a key-value pair in the DynHashmap
Error dynhashmap_set(DynHashmap* hashmap, char* key, void* value) {
    if(hashmap == NULL || hashmap->keys.data == NULL || hashmap->values.data == NULL || hashmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if(key == NULL || value == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;

    // resize if required
    err = dynhashmap_resize(hashmap); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // calculate the index using the hash function
    Int64 index = hash(key) % hashmap->capacity;

    // loop from the index to the end of the array
    for (Int64 i = index; i < hashmap->capacity; i++) {
        Bool is_available;
        err = dynarray_get(&hashmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the slot is available, insert the key-value pair
        if (is_available) {
            // insert the key
            err = dynarray_set(&hashmap->keys, i, key, sizeof(FxString));
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            // insert the value
            err = dynarray_set(&hashmap->values, i, value, hashmap->value_size);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            // mark the slot as unavailable
            err = dynarray_set(&hashmap->available, i, &(Bool){false}, sizeof(Bool));
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            hashmap->len++;
            return (Error){.ok = true};
        }

        // if the slot is not available, check if the key is the same as the key we are looking for
        FxString current_key;
        err = dynarray_get(&hashmap->keys, i, &current_key);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the key is the same, update the value
        if (strcmp(current_key.data, key) == 0) {
            err = dynarray_set(&hashmap->values, i, value, hashmap->value_size);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
            return (Error){.ok = true};
        }
    }

    // if we reach here, it means we have reached the end of the array
    return snitch("Out of capacity", __LINE__, __FILE__);
}


Error dynhashmap_has_key(DynHashmap* hmap, const char* key, Bool* result) {
    if (hmap == NULL || hmap->keys.data == NULL || hmap->values.data == NULL || hmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (result == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;

    // calculate the index using the hash function
    Int64 index = hash(key) % hmap->capacity;

    // loop from the index to the end of the array
    for (Int64 i = index; i < hmap->capacity; i++) {
        Bool is_available;
        err = dynarray_get(&hmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the slot is available, the key is not in the hashmap
        if (is_available) {
            *result = false;
            return (Error){.ok = true};
        }

        // if the slot is not available, check if the key is the same as the key we are looking for
        FxString current_key;
        err = dynarray_get(&hmap->keys, i, &current_key);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the key is the same, get the value
        if (strcmp(current_key.data, key) == 0) {
            *result = true;
            return (Error){.ok = true};
        }
    }

    *result = false;
    return (Error){.ok = true};
}


// TODO - use has-Key for the lookup?
// Get a value from the DynHashmap
Error dynhashmap_get(DynHashmap* hashmap, const char* key, void* out) {
    if (hashmap == NULL || hashmap->keys.data == NULL || hashmap->values.data == NULL || hashmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;

    // calculate the index using the hash function
    Int64 index = hash(key) % hashmap->capacity;

    // loop from the index to the end of the array
    for (Int64 i = index; i < hashmap->capacity; i++) {
        Bool is_available;
        err = dynarray_get(&hashmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the slot is available, the key is not in the hashmap
        if (is_available) return snitch("Key not found", __LINE__, __FILE__);

        // if the slot is not available, check if the key is the same as the key we are looking for
        FxString current_key;
        err = dynarray_get(&hashmap->keys, i, &current_key);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        // if the key is the same, get the value
        if (strcmp(current_key.data, key) == 0) {
            err = dynarray_get(&hashmap->values, i, out);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
            return (Error){.ok = true};
        }
    }

    return snitch("Key not found", __LINE__, __FILE__);
}


// Remove a key-value pair from the DynHashmap. use get method internally
Error dynhashmap_remove(DynHashmap* hashmap, const char* key, void* out) {
    if (hashmap == NULL || hashmap->keys.data == NULL || hashmap->values.data == NULL || hashmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    
    err = dynhashmap_get(hashmap, key, out);
    if(!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Set the slot as available
    Int64 index = hash(key) % hashmap->capacity;
    err = dynarray_set(&hashmap->available, index, &(Bool){true}, sizeof(Bool));
    if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Decrement the length
    hashmap->len--;

    return (Error){.ok = true};
}

// Run a function On Each pair. Mainly to iterate over the dynarrays
// Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another hashmap
// For reducing, the acc is any value type
Error dynhashmap_oneach(DynHashmap* hmap, void* acc, Error (*fn)(Int64, Int64, void*, FxString, void*)) {
    if (hmap == NULL || hmap->keys.data == NULL || hmap->values.data == NULL || hmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (hmap->keys.data == NULL || hmap->values.data == NULL || hmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);

    if (acc == NULL || fn == NULL) return snitch("Invalid input", __LINE__, __FILE__);
    
    Error err;
    for (Int64 i = 0; i < hmap->len; i++) {
        Bool is_available;
        err = dynarray_get(&hmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        if (!is_available) {
            FxString key;
            err = dynarray_get(&hmap->keys, i, &key);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            void* value;
            err = dynarray_get(&hmap->values, i, &value);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            err = fn(hmap->len, i, acc, key, value);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
        }
    }

    return (Error){ .ok = true };
}

Error dynhashmap_list_keys(DynHashmap* hmap, DynArray* keys) {
    if (hmap == NULL || hmap->keys.data == NULL || hmap->values.data == NULL || hmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (keys == NULL || keys->data == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    for (Int64 i = 0; i < hmap->len; i++) {
        Bool is_available;
        err = dynarray_get(&hmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        if (is_available) {
            FxString key;
            err = dynarray_get(&hmap->keys, i, &key);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

            err = dynarray_push(keys, &key, sizeof(FxString));
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
        }
    }

    return (Error){ .ok = true };
}

Error dynhashmap_list_values(DynHashmap* hmap, DynArray* values) {
    if (hmap == NULL || hmap->keys.data == NULL || hmap->values.data == NULL || hmap->available.data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (values->data == NULL || values->data == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    for (Int64 i = 0; i < hmap->len; i++) {
        Bool is_available;
        err = dynarray_get(&hmap->available, i, &is_available);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        if (is_available) {
            void* value = (Byte*)hmap->values.data + i * hmap->values.item_size;

            err = dynarray_push(values, value, hmap->values.item_size);
            if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
        }
    }

    return (Error){ .ok = true };
}