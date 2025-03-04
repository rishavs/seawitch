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
    int64_t   item_size;
    int64_t   len;
    int64_t   capacity;
} DynArray; // dynarray owns the data

// Create a new dynamic array.
DynArray* dynarray_create(Types item_type, int64_t item_size, int64_t initial_capacity);

// Push an item to the end of the dynarray
Error dynarray_push(DynArray* dynarray, void* item, int64_t item_size);

// Pop an item from the end of the dynarray
Error dynarray_pop(DynArray* dynarray, void* out);

// Get the value at the index
Error dynarray_get(DynArray* dynarray, int64_t index, void* out);

// Set the value at the index
Error dynarray_set (DynArray* dynarray, int64_t index, void* item, int64_t item_size);

// Slice the dynarray between two indices. Inclusive
Error dynarray_slice(DynArray* dynarray, DynArray* slice, int64_t start, int64_t end);

// Join multiple dynarrays into a single dynarray
Error dynarray_join(DynArray* joined, int64_t n, ...) ;

// Append a dynarray to another dynarray
Error dynarray_append(DynArray* dest, DynArray* src);

// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. or for printing.
// For mapping, the acc is another array
// For reducing, the acc is any value type
Error dynarray_oneach(DynArray* dynarray, void* acc, Error (*fn)(int64_t, int64_t, void*, void*));

// Filter function. Iterates over the dynarray and applies the predicate to each element.
// Returns a new array with only thos elements that satisfy the predicate
Error dynarray_filter(DynArray* source, DynArray* filtered, Error (*fn)(int64_t, int64_t, void*, Bool*));

// Sort the dynarray using a comparison function
Error dynarray_sort(DynArray* dynarray, DynArray* sorted, int (*cmp)(const void *, const void *));

// Compare two dynarrays using a comparison function
Error dynarray_compare(DynArray* arr1, DynArray* arr2, Error (*cmp)(void*, void*, Bool*), Bool* result);

// Check if a dynarray has a subarray at a given position
Bool dynarray_has_subarray(DynArray* dynarray, int64_t pos, DynArray* subarray);

// Find the index of an item in the dynarray
// Uses a comparison function to compare items
Error dynarray_find_first(DynArray* dynarray, int64_t from, void* item, int64_t* at, Error (*cmp)(void*, void*, Bool*));

#endif // SEAWITCH_DYNARRAYS_H