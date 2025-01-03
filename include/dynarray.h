#ifndef SEAWITCH_DYNARRAYS_H
#define SEAWITCH_DYNARRAYS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seawitch.h"

#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 8
#endif

// Dynamicarray is a low level byte array that can store and own any type of data
typedef struct {
    Byte*   data; //data is stored as bytes
    Types   item_type;
    Int64   item_size;
    Int64   len;
    Int64   capacity;
} DynArray; // dynarray owns the data

// Create a new dynamic array.
DynArray* dynarray_create(Types item_type, Int64 item_size, Int64 initial_capacity);

// Push an item to the end of the dynarray
Error dynarray_push(DynArray* dynarray, void* item, Int64 item_size);

// Pop an item from the end of the dynarray
Error dynarray_pop(DynArray* dynarray, void* out);

// Get the value at the index
Error dynarray_get(DynArray* dynarray, Int64 index, void* out);

// Set the value at the index
Error dynarray_set (DynArray* dynarray, Int64 index, void* item, Int64 item_size);

// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. or for printing.
// For mapping, the acc is another array
// For reducing, the acc is any value type
Error dynarray_oneach(DynArray* dynarray, void* acc, Error (*fn)(Int64, Int64, void*, void*));

// Slice the dynarray between two indices. Inclusive
Error dynarray_slice(DynArray* dynarray, DynArray* slice, Int64 start, Int64 end);

// Join multiple dynarrays into a single dynarray
DynArray *dynarray_join(Int64 n, ...);


// Filter function. Iterates over the dynarray and applies the predicate to each element.
// Returns a new array with only thos elements that satisfy the predicate
DynArray* dynarray_filter(DynArray* dynarray, Bool (*predicate)(void*));

// Sort the dynarray using a comparison function
DynArray* dynarray_sort(DynArray* dynarray, int (*cmp)(const void*, const void*));

// Compare two dynarrays using a comparison function
Bool dynarray_compare(DynArray* dynarray1, DynArray* dynarray2, Bool (*cmp)(void*, void*));

// Check if a dynarray has a subarray at a given position
Bool dynarray_has_subarray(DynArray* dynarray, Int64 pos, DynArray* subarray);

// Find the index of an item in the dynarray
// Uses a comparison function to compare items
Bool dynarray_find(DynArray* dynarray, Int64* out,  void* item, Bool (*cmp)(void*, void*));

#endif // SEAWITCH_DYNARRAYS_H