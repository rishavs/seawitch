#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "dynarray.h"
#include "seawitch.h"

#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 8
#endif

// TODO - all helper functions should also take in the index of the item

// Create a new dynamic array.
DynArray* dynarray_create(Types item_type, size_t item_size, size_t initial_capacity) {
    DynArray *dynarray = calloc(1, sizeof(DynArray));
    if (dynarray == NULL) return print_error_return_null(__FILE__, __LINE__);

    dynarray->item_type = item_type;
    dynarray->item_size = item_size;
    dynarray->len = 0;
    dynarray->capacity = initial_capacity ? initial_capacity : INITIAL_CAPACITY;
    
    dynarray->data = calloc(dynarray->capacity, dynarray->item_size);
    if (dynarray->data == NULL) return print_error_return_null(__FILE__, __LINE__);

    return dynarray;
}

Bool dynarray_push(DynArray* dynarray, Gen_ref item) {
    if (!dynarray || !item) return print_error_return_false(__FILE__, __LINE__);

    if (dynarray->len == dynarray->capacity) {
        size_t new_capacity = dynarray->capacity * 2;
        if (new_capacity < dynarray->capacity || new_capacity * dynarray->item_size < new_capacity) { // Overflow check
            return print_error_return_false(__FILE__, __LINE__); // Handle overflow error
        }
        dynarray->capacity = new_capacity;
        dynarray->data = realloc(dynarray->data, dynarray->capacity * dynarray->item_size);
        if (!dynarray->data) return print_error_return_false(__FILE__, __LINE__);
    }

    memcpy((Byte*)dynarray->data + dynarray->len * dynarray->item_size, item, dynarray->item_size);
    dynarray->len++;

    return true;
}

Bool dynarray_pop(DynArray* dynarray, Gen_ref out) {
    if (!dynarray || !out) return print_error_return_false(__FILE__, __LINE__);

    // If the dynarray is empty
    if (dynarray->len == 0) return print_error_return_false(__FILE__, __LINE__);

    // Remove the last element from the dynarray
    memcpy(out, (Byte*)dynarray->data + (dynarray->len - 1) * dynarray->item_size, dynarray->item_size);  
    dynarray->len--;

    return true;
}

// Get the value at the index
Gen_ref dynarray_get(DynArray* dynarray, size_t index, Gen_ref out) {
    if (!dynarray || !out) return print_error_return_null(__FILE__, __LINE__);

    // check if the index is out of bounds
    if (index >= dynarray->len) return NULL;

    memcpy(out, (Byte *)dynarray->data + index * dynarray->item_size, dynarray->item_size);

    return out;    
}

// Set the value at the index
Bool dynarray_set (DynArray* dynarray, size_t index, Gen_ref item) {
    if (!dynarray || !item) return print_error_return_false(__FILE__, __LINE__);

    // If the index is out of bounds
    if (index >= dynarray->len) return print_error_return_false(__FILE__, __LINE__);

    memcpy((Byte*)dynarray->data + index * dynarray->item_size, item, dynarray->item_size); 

    return true;
}

// Slice the dynarray from start to end. Both inclusive
DynArray* dynarray_slice(DynArray* dynarray, size_t start, size_t end) {
    if (!dynarray) return print_error_return_null(__FILE__, __LINE__);

    // Bounds checking
    if (start >= dynarray->len || end < start || end >= dynarray->len) return print_error_return_null(__FILE__, __LINE__);

    // Calculate new length
    size_t new_len = end - start + 1;
            
    // Create new array
    DynArray* new_dynarray = dynarray_create(dynarray->item_type, dynarray->item_size, new_len * 2);
    if (!new_dynarray) return print_error_return_null(__FILE__, __LINE__);
    
    new_dynarray->len = new_len;

    // Perform the copy
    memcpy(
        new_dynarray->data,
        (Byte*)dynarray->data + (start * dynarray->item_size),
        new_len * dynarray->item_size
    );

    return new_dynarray;
}

DynArray *dynarray_join(size_t n, ...) {
    if (n <= 0) return print_error_return_null(__FILE__, __LINE__);

    va_list args;
    va_start(args, n);

    DynArray* first_dynarray = va_arg(args, DynArray*);
    if (!first_dynarray) {
        va_end(args);
        return print_error_return_null(__FILE__, __LINE__);
    }

    Types item_type = first_dynarray->item_type;
    size_t item_size = first_dynarray->item_size;

    // Check type and size consistency
    for (size_t i = 0; i < n ; i++) {
        DynArray *dynarray = va_arg(args, DynArray*);
        if (!dynarray || dynarray->item_type != item_type || dynarray->item_size != item_size) {
            va_end(args);
            return print_error_return_null(__FILE__, __LINE__);
        }
    }
    va_end(args);

    // Calculate total length
    va_start(args, n);
    size_t total_len = 0;
    for (size_t i = 0; i < n; i++) {
        DynArray *dynarray = va_arg(args, DynArray *);
        if (dynarray) {
            total_len += dynarray->len;
        }
    }
    va_end(args);

    // Create joined dynarray
    DynArray *joined_dynarray = dynarray_create(item_type, item_size, total_len * 2); // Double the capacity
    joined_dynarray->len = total_len; // Set the correct length

    // Copy elements
    va_start(args, n);
    size_t offset = 0;
    for (size_t i = 0; i < n; i++) {
        DynArray *dynarray = va_arg(args, DynArray *);
        if (dynarray) {
            memcpy(
                joined_dynarray->data + offset * item_size, 
                dynarray->data, 
                dynarray->len * item_size
            );
            offset += dynarray->len;
        }
    }
    va_end(args);

    return joined_dynarray;
}

// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another array
// For reducing, the acc is any value type
Bool dynarray_oneach(DynArray* dynarray, Gen_ref acc, void (*fn)(size_t, Gen_ref, Gen_ref)) {
    if (!dynarray || !fn || !acc) return print_error_return_false(__FILE__, __LINE__);

    for (size_t i = 0; i < dynarray->len; i++) {
        Gen_ref item = (Byte*)dynarray->data + i * dynarray->item_size;
        fn(i, acc, item);  // fn should modify result in place
    }

    return true;
}

// Filter function. Iterates over the dynarray and applies the predicate to each element.
// Returns a new array with only thos elements that satisfy the predicate
DynArray* dynarray_filter(DynArray* dynarray, bool (*predicate)(Gen_ref)) {
    if (!dynarray || !predicate) return print_error_return_null(__FILE__, __LINE__);
    
    DynArray* result = dynarray_create(dynarray->item_type, dynarray->item_size, dynarray->len);
    if (!result) return print_error_return_null(__FILE__, __LINE__);
    
    for (size_t i = 0; i < dynarray->len; i++) {
        Gen_ref item = (Byte*)dynarray->data + i * dynarray->item_size;
        if (predicate(item)) {
            dynarray_push(result, item);
        }
    }
    
    return result;
}

DynArray* dynarray_sort(DynArray* dynarray, int (*cmp)(const void *, const void *)) { // int (*cmp)
    if (!dynarray || !cmp) return print_error_return_null(__FILE__, __LINE__);

    DynArray* result = dynarray_create(dynarray->item_type, dynarray->item_size, dynarray->len);
    if (!result) return print_error_return_null(__FILE__, __LINE__);

    memcpy(result->data, dynarray->data, dynarray->len * dynarray->item_size);
    result->len = dynarray->len;

    qsort(result->data, result->len, result->item_size, cmp); // Call qsort directly

    return result;
}

Bool dynarray_compare(DynArray* dynarray1, DynArray* dynarray2, Bool (*cmp)(Gen_ref, Gen_ref)) {
    if (!dynarray1 || !dynarray2 || !cmp) return print_error_return_false(__FILE__, __LINE__);

    if (dynarray1->len != dynarray2->len) return false;

    for (size_t i = 0; i < dynarray1->len; i++) {
        Gen_ref item1 = (Byte*)dynarray1->data + i * dynarray1->item_size;
        Gen_ref item2 = (Byte*)dynarray2->data + i * dynarray2->item_size;
        if (!cmp(item1, item2)) return false;
    }

    return true;
}

Bool dynarray_has_subarray(DynArray* dynarray, size_t pos, DynArray* subarray) {
    if (!dynarray || !subarray) return print_error_return_false(__FILE__, __LINE__);

    if (pos >= dynarray->len) return print_error_return_false(__FILE__, __LINE__);

    if (subarray->len > dynarray->len - pos) return print_error_return_false(__FILE__, __LINE__);

    for (size_t i = 0; i < subarray->len; i++) {
        Gen_ref item1 = (Byte*)dynarray->data + (pos + i) * dynarray->item_size;
        Gen_ref item2 = (Byte*)subarray->data + i * subarray->item_size;
        if (memcmp(item1, item2, dynarray->item_size) != 0) return false;
    }

    return true;
}

// Find the index of an item in the dynarray
// Uses a comparison function to compare items
Bool dynarray_find(DynArray* dynarray, size_t* out,  Gen_ref item, Bool (*cmp)(Gen_ref, Gen_ref)) {
    if (!dynarray || !out || !item || !cmp) return print_error_return_false(__FILE__, __LINE__);

    for (size_t i = 0; i < dynarray->len; i++) {
        Gen_ref current = (Byte*)dynarray->data + i * dynarray->item_size;
        if (cmp(current, item)) {
            *out = i;
            return true;
        }
    }

    return false;
}