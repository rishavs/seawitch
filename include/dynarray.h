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
    Byte*       data; //data is stored as bytes
    Types       item_type;
    size_t       item_size;
    size_t       len;
    size_t       capacity;
} DynArray; // dynarray owns the data

// Create a new dynamic array.
DynArray* dynarray_create(Types item_type, size_t item_size, size_t initial_capacity);

// Push an item to the end of the dynarray
Bool dynarray_push(DynArray* dynarray, Gen_ref item);

// Pop an item from the end of the dynarray
Bool dynarray_pop(DynArray* dynarray, Gen_ref out);

// Get the value at the index
Gen_ref dynarray_get(DynArray* dynarray, size_t index, Gen_ref out);

// Set the value at the index
Bool dynarray_set (DynArray* dynarray, size_t index, Gen_ref item);

// Slice the dynarray between two indices. Inclusive
DynArray* dynarray_slice(DynArray* dynarray, size_t start, size_t end);

// Join multiple dynarrays into a single dynarray
DynArray *dynarray_join(size_t n, ...);

// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. or for printing.
// For mapping, the acc is another array
// For reducing, the acc is any value type
Bool dynarray_oneach(DynArray* dynarray, Gen_ref acc, void (*fn)(size_t, Gen_ref, Gen_ref));

// Filter function. Iterates over the dynarray and applies the predicate to each element.
// Returns a new array with only thos elements that satisfy the predicate
DynArray* dynarray_filter(DynArray* dynarray, bool (*predicate)(Gen_ref));

// Sort the dynarray using a comparison function
DynArray* dynarray_sort(DynArray* dynarray, int (*cmp)(const void *, const void *));

// Compare two dynarrays using a comparison function
Bool dynarray_compare(DynArray* dynarray1, DynArray* dynarray2, Bool (*cmp)(Gen_ref, Gen_ref));

// Check if a dynarray has a subarray at a given position
Bool dynarray_has_subarray(DynArray* dynarray, size_t pos, DynArray* subarray);

// Find the index of an item in the dynarray
// Uses a comparison function to compare items
Bool dynarray_find(DynArray* dynarray, size_t* out,  Gen_ref item, Bool (*cmp)(Gen_ref, Gen_ref));

#endif // SEAWITCH_DYNARRAYS_H