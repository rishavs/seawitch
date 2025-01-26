#ifndef SEAWITCH_DYNHASHMAPS_H
#define SEAWITCH_DYNHASHMAPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seawitch.h"
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
    DynArray keys; // keys are FxStrings
    DynArray values;   // values are generic
    DynArray available;   // array of bools to track if slot is available. 
    // Removing a pair only marks the slot as available. the size of the array remains the same
    
    Types value_type;
    Int64 value_size;
    Int64 len;
    Int64 capacity;
} DynHashmap;

// Create a new dictionary
// Note: the dict will only grow, as required, and not shrink.
DynHashmap* dynhashmap_create(Types value_type, Int64 value_size, Int64 initial_capacity);

Error dynhashmap_resize(DynHashmap* hashmap) ;
// Set a key-value pair in the DynHashmap
Error dynhashmap_set(DynHashmap* hashmap, char* key, void* value);

Error dynhashmap_has_key(DynHashmap* hmap, const char* key, Bool* result);
// TODO - use has-Key for the lookup?
// Get a value from the DynHashmap
Error dynhashmap_get(DynHashmap* hashmap, const char* key, void* out);

// Remove a key-value pair from the DynHashmap. use get method internally
Error dynhashmap_remove(DynHashmap* hashmap, const char* key, void* out);

// Run a function On Each pair. Mainly to iterate over the dynarrays
// Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another hashmap
// For reducing, the acc is any value type
Error dynhashmap_oneach(DynHashmap* hmap, void* acc, Error (*fn)(Int64, Int64, void*, FxString, void*));

Error dynhashmap_list_keys(DynHashmap* hmap, DynArray* keys);

Error dynhashmap_list_values(DynHashmap* hmap, DynArray* values);

#endif // SEAWITCH_DYNHASHMAPS_H