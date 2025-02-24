#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "dynarray.h"
#include "dynhashmap.h"
#include "seawitch.h"

#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR 0.75
#define DICT_MAX_KEY_LENGTH 256

// typedef struct {
//     FxString* keys;
//     Byte*   data; //data is stored as bytes
//     Bool*   available;
//     Int64*  pdistance; // New: Probe distance array
//     Int64*  hvalue; // New: Hash values array

//     Types   item_type;
//     Int64   item_size;
//     Int64   len;
//     Int64   capacity;
// } DynHashmap;

// FNV-1a hash function
static Int64 hash(const char* str) {
    Int64 hash = 0xcbf29ce484222325LL;
    unsigned char* s = (unsigned char*)str;
    
    while (*s) {
        hash ^= *(Int64*)s;
        hash *= 0x100000001b3LL;
        s += 4;
    }
    
    hash ^= hash >> 32; // Additional mixing
    return hash;
}

static inline Int64 find_next_available_slot(DynHashmap* hmap, const char* key) {
    Int64 base_index = hash(key) % hmap->capacity;
    Int64 i = 0;
    Int64 index;
    
    while (i < hmap->capacity) {
        // Quadratic probing formula: (base + i + i*i) % capacity
        index = (base_index + i*i) % hmap->capacity;
        if (hmap->available[index]) return index;

        i++;
    }
    return -1;
}

static inline Int64 find_key(DynHashmap* hmap, const char* key) {
    Int64 base_index = hash(key) % hmap->capacity;
    Int64 i = 0;
    Int64 index;
    
    while (i < hmap->capacity) {
        // Quadratic probing formula: (base + i + i*i) % capacity
        index = (base_index + i*i) % hmap->capacity;
        if (hmap->available[index] == true ) break; 
        
        if (strcmp(hmap->keys[index].data, key) == 0) return index;

        i++;
    }
    return -1;
}

DynHashmap* dynhashmap_create(Types value_type, Int64 value_size, Int64 initial_capacity) {
    DynHashmap* hmap = calloc(1, sizeof(DynHashmap));
    if (hmap ==  NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    Int64 optimal_capacity = initial_capacity > 0 ? initial_capacity * 2 : DICT_INITIAL_CAPACITY; 

    Error err;

    hmap->keys      = calloc(optimal_capacity, sizeof(FxString));
    hmap->data      = calloc(optimal_capacity, value_size);
    hmap->available = calloc(optimal_capacity, sizeof(Bool));
    hmap->insert_order = calloc(optimal_capacity, sizeof(Int64));
    if (hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL || hmap->insert_order == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    hmap->item_type = value_type;
    hmap->item_size = value_size;
    hmap->len = 0;
    hmap->capacity = optimal_capacity;

    // Initialize available array to true
    for (Int64 i = 0; i < optimal_capacity; i++) {
        hmap->available[i] = true;        
    }

    return hmap;
}

Error dynhashmap_set(DynHashmap* hmap, char* key, void* value) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL || value == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err = dynhashmap_resize(hmap);
    if (!err.ok) return err;

    Int64 index = find_next_available_slot(hmap, key);
    if (index == -1) return snitch("Out of capacity", __LINE__, __FILE__);

    // Check for existing key first
    Int64 existing = find_key(hmap, key);
    if (existing != -1) {
        void* res = memmove((Byte*)hmap->data + existing * hmap->item_size, 
                           value, hmap->item_size);
        if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));
        return (Error){.ok = true};
    }

    // Insert new key-value pair
    hmap->keys[index] = fxstring_create(key);
    void* res = memmove((Byte*)hmap->data + index * hmap->item_size, 
                        value, hmap->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    hmap->available[index] = false;
    hmap->len++;

    // update the insert order
    hmap->insert_order[index] = hmap->len;

    return (Error){.ok = true};
}

Error dynhashmap_resize(DynHashmap* hmap) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);

    if ((float)hmap->len / hmap->capacity < DICT_LOAD_FACTOR) 
        return (Error){.ok = true};

    Int64 new_capacity = hmap->capacity * 2;
    DynHashmap* new_hmap = dynhashmap_create(hmap->item_type, 
                                           hmap->item_size, 
                                           new_capacity);
    
    for (Int64 i = 0, transferred = 0; i < hmap->capacity && transferred < hmap->len; i++) {
        if (!hmap->available[i]) {
            Error err = dynhashmap_set(new_hmap, hmap->keys[i].data, 
                                     (Byte*)hmap->data + i * hmap->item_size);
            if (!err.ok) return err;
            transferred++;
        }
    }
    
    // Transfer ownership
    hmap->keys          = new_hmap->keys;
    hmap->data          = new_hmap->data;
    hmap->available     = new_hmap->available;
    hmap->capacity      = new_hmap->capacity;
    hmap->len           = new_hmap->len;
    hmap->insert_order  = new_hmap->insert_order;

    return (Error){.ok = true};
}

// Update these functions to use the helper functions
Error dynhashmap_get(DynHashmap* hmap, const char* key, void* out) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL) return snitch("Null input", __LINE__, __FILE__);

    Int64 index = find_key(hmap, key);
    if (index == -1) return (Error){.ok = true};

    void* res = memmove(out, (Byte*)hmap->data + index * hmap->item_size, 
                        hmap->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));
    
    return (Error){.ok = true};
}

Error dynhashmap_remove(DynHashmap* hmap, const char* key, void* out) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL) return snitch("Null input", __LINE__, __FILE__);

    Int64 index = find_key(hmap, key);
    if (index == -1) return (Error){.ok = true};

    void* res = memmove(out, (Byte*)hmap->data + index * hmap->item_size, 
                        hmap->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    hmap->available[index] = true;
    hmap->len--;
    
    return (Error){.ok = true};
}


Error dynhashmap_has_key(DynHashmap* hmap, const char* key, Bool* result) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (key == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (result == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    Int64 index = find_key(hmap, key);
    if (index == -1) return (Error){.ok = true};

    *result = true;
    return (Error){.ok = true};
}

// Run a function On Each pair. Mainly to iterate over the dynarrays
// Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another hashmap
// For reducing, the acc is any value type
Error dynhashmap_oneach(DynHashmap* hmap, void* acc, Error (*fn)(Int64, Int64, void*, FxString, void*)) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (acc == NULL || fn == NULL) return snitch("Null input", __LINE__, __FILE__);
    
    Error err;
    Int64 len_checked = 0; // if we have checked all the keys, no need to continue with the remaining capacity

    for (Int64 i = 0; i < hmap->capacity; i++) {
        if (len_checked >= hmap->len) break;
        if (hmap->available[i] == true) continue;

        FxString key = hmap->keys[i];
        void* value = (Byte*)hmap->data + i * hmap->item_size;
        err = fn(hmap->len, len_checked, acc, key, value); // sending len_checked instead of i, as i also includes the unavailable slots
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        len_checked++;        
    }
    return (Error){ .ok = true };
}


Error dynhashmap_list_keys(DynHashmap* hmap, DynArray* keys) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (keys == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    Int64 len_checked = 0; // if we have checked all the keys, no need to continue with the remaining capacity
    for (Int64 i = 0; i < hmap->capacity && len_checked < hmap->len; i++) {
        if (len_checked >= hmap->len) break;
        if (hmap->available[i] == true) continue;

        err = dynarray_push(keys, &hmap->keys[i], sizeof(FxString));
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

        len_checked++;
    }

    return (Error){ .ok = true };
}



// Comparison function for qsort to sort by insertion order
static int compare_insert_order(const void* a, const void* b) {
    Int64 index_a = *(Int64*)a;
    Int64 index_b = *(Int64*)b;
    return index_a - index_b; // Ascending order
}

Error dynhashmap_list_keys_in_order(DynHashmap* hmap, DynArray* keys) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL || hmap->insert_order == NULL) {
        return snitch("Null input", __LINE__, __FILE__);
    }
    if (keys == NULL) {
        return snitch("Null input", __LINE__, __FILE__);
    }

    Error err;

    // 1. Create an array of indices of inserted elements
    Int64* inserted_indices = malloc(sizeof(Int64) * hmap->len);
    if (inserted_indices == NULL) {
        return snitch("Memory error", __LINE__, __FILE__);
    }

    Int64 inserted_count = 0;
    for (Int64 i = 0; i < hmap->capacity; i++) {
        if (!hmap->available[i]) {
            inserted_indices[inserted_count++] = i;
        }
    }

    // 2. Sort the indices based on insertion order
    qsort(inserted_indices, hmap->len, sizeof(Int64), compare_insert_order);

    // 3. Add the keys to the DynArray in the sorted order
    for (Int64 i = 0; i < hmap->len; i++) {
        Int64 index = inserted_indices[i];
        err = dynarray_push(keys, &hmap->keys[index], sizeof(FxString));
        if (!err.ok) {
            free(inserted_indices); // Important: Free allocated memory on error
            return snitch(err.message.data, __LINE__, __FILE__);
        }
    }

    free(inserted_indices); // Free the allocated indices array

    return (Error){.ok = true};
}

Error dynhashmap_list_values(DynHashmap* hmap, DynArray* values) {
    if (hmap == NULL || hmap->keys == NULL || hmap->data == NULL || hmap->available == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (values == NULL || values->data == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;
    Int64 len_checked = 0; // if we have checked all the keys, no need to continue with the remaining capacity
    
    for (Int64 i = 0; i < hmap->capacity && len_checked < hmap->len; i++) {
        if (len_checked >= hmap->len) break;
        if (hmap->available[i] == true) continue;

        len_checked++;
        void* value = (Byte*)hmap->data + i * hmap->item_size;

        err = dynarray_push(values, value, hmap->item_size);
        if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    }
    
    return (Error){ .ok = true };
}
