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
DynArray* dynarray_create(Types item_type, Int64 item_size, Int64 initial_capacity) {
    DynArray *dynarray = calloc(1, sizeof(DynArray));
    if (dynarray == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    dynarray->item_type = item_type;
    dynarray->item_size = item_size;
    dynarray->len = 0;
    dynarray->capacity = initial_capacity ? initial_capacity : INITIAL_CAPACITY;
    
    dynarray->data = calloc(dynarray->capacity, dynarray->item_size);
    if (dynarray->data == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    return dynarray;
}

Error dynarray_push(DynArray* dynarray, void* item, Int64 item_size) {
    if (dynarray == NULL || dynarray->data == NULL ) return snitch("Null input", __LINE__, __FILE__);
    if (item == NULL) return snitch("Invalid input", __LINE__, __FILE__);

    if (dynarray->item_size != item_size || item_size == 0) return snitch("Invalid input", __LINE__, __FILE__);

    if (dynarray->len == dynarray->capacity) {
        Int64 new_capacity = dynarray->capacity * 2;
        if (new_capacity < dynarray->capacity || new_capacity * dynarray->item_size < new_capacity) { // Overflow check
            return snitch("Integer overflow", __LINE__, __FILE__);
        }
        dynarray->capacity = new_capacity;
        dynarray->data = realloc(dynarray->data, dynarray->capacity * dynarray->item_size);
        if (!dynarray->data) fatal(snitch("Memory error", __LINE__, __FILE__));
    }

    void* res = memmove((Byte*)dynarray->data + dynarray->len * dynarray->item_size, item, dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    dynarray->len++;

    return (Error){ .ok = true };
}

Error dynarray_pop(DynArray* dynarray, void* out) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL ) return snitch("Invalid input", __LINE__, __FILE__);

    // If the dynarray is empty
    if (dynarray->len == 0) return snitch("Out of bounds input", __LINE__, __FILE__);

    // Remove the last element from the dynarray
    // memcpy(out, (Byte*)dynarray->data + (dynarray->len - 1) * dynarray->item_size, dynarray->item_size);
    // if (!out) return print_error_return_false(__FILE__, __LINE__);
    void* res = memmove(out, (Byte*)dynarray->data + (dynarray->len - 1) * dynarray->item_size, dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    dynarray->len--;

    return (Error){ .ok = true };
}

// Get the value at the index
Error dynarray_get(DynArray* dynarray, Int64 index, void* out) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (out == NULL) return snitch("Invalid input", __LINE__, __FILE__);

    // check if the index is out of bounds
    if (index < 0 || index >= dynarray->len) return snitch("Out of bounds input", __LINE__, __FILE__);

    // If the dynarray is empty
    if (dynarray->len == 0) return snitch("Out of bounds input", __LINE__, __FILE__);

    // memcpy(out, (Byte *)dynarray->data + index * dynarray->item_size, dynarray->item_size);
    // if (!out) return print_error_return_null(__FILE__, __LINE__);

    void* res = memmove(out, (Byte *)dynarray->data + index * dynarray->item_size, dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    return (Error){ .ok = true };
}

// Set the value at the index
Error dynarray_set (DynArray* dynarray, Int64 index, void* item, Int64 item_size) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (item == NULL) return snitch("Invalid input", __LINE__, __FILE__);

    // check if the index is out of bounds
    if (index < 0 || index >= dynarray->len) return snitch("Out of bounds input", __LINE__, __FILE__);

    // check if the item size is correct
    if (dynarray->item_size != item_size || item_size == 0) return snitch("Invalid input", __LINE__, __FILE__);

    // If the dynarray is empty
    if (dynarray->len == 0) return snitch("Out of bounds input", __LINE__, __FILE__);
    
    // memcpy((Byte*)dynarray->data + index * dynarray->item_size, item, dynarray->item_size); 
    // if (!item) return print_error_return_false(__FILE__, __LINE__);

    void* res = memmove((Byte*)dynarray->data + index * dynarray->item_size, item, dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));
    
    return (Error){ .ok = true };
}

// Slice the dynarray from start to end. Both inclusive
Error dynarray_slice(DynArray* dynarray, DynArray* slice, Int64 start, Int64 end) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (slice == NULL || slice->data == NULL) return snitch("Null input", __LINE__, __FILE__);

    // Bounds checking
    if (start < 0 || end < 0 || end < start) return snitch("Out of bounds input", __LINE__, __FILE__);
    if (start >= dynarray->len || end >= dynarray->len) return snitch("Out of bounds input", __LINE__, __FILE__);

    // If slice is not empty, throw error
    if (slice->len > 0) return snitch("Invalid data", __LINE__, __FILE__);

    // Update the slice array
    slice->len = end - start + 1;
    slice->capacity = slice->len * 2; // Double the capacity
    slice->data = calloc(slice->capacity, dynarray->item_size);    
    if (!slice->data) fatal (snitch("Memory error", __LINE__, __FILE__));

    void* res = memmove(slice->data, (Byte*)dynarray->data + (start * dynarray->item_size), slice->len * dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    return (Error){ .ok = true };
}

Error dynarray_append(DynArray* dest, DynArray* src) {
    if (dest == NULL || dest->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (src == NULL || src->data == NULL) return snitch("Null input", __LINE__, __FILE__);

    // Check for type and size consistency
    if (dest->item_type != src->item_type || dest->item_size != src->item_size) return snitch("Invalid input", __LINE__, __FILE__);

    // Update the dest dynarray (allocate memory)
    dest->len += src->len;
    dest->capacity = dest->len > 0 ? dest->len * 2: 1; // Double the capacity or 1 if len is 0.
    dest->data = realloc(dest->data, dest->capacity * dest->item_size);
    if (!dest->data) return snitch("Memory error", __LINE__, __FILE__);

    // Copy elements
    void* res = memmove((Byte*)dest->data + (dest->len - src->len) * dest->item_size, src->data, src->len * src->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    return (Error){.ok = true};
}

Error dynarray_join(DynArray* joined, Int64 n, ...) {

    if (joined == NULL || joined->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (n <= 0) return snitch("Out of bounds input", __LINE__, __FILE__);

    va_list args;
    va_start(args, n);

    // Get the first dynarray (outside the loop)
    DynArray* first_dynarray = va_arg(args, DynArray*);
    if (first_dynarray == NULL || first_dynarray->data == NULL) {
        va_end(args);
        return snitch("Null input", __LINE__, __FILE__);
    }

    Types item_type = first_dynarray->item_type;
    Int64 item_size = first_dynarray->item_size;

    Int64 total_len = 0;

    //Check for null inputs and type and size consistancy
    va_list args_len_check;
    va_copy(args_len_check, args);

    for (Int64 i = 0; i < n - 1; i++) { // n-1 because we already processed the first
        DynArray* dynarray = va_arg(args_len_check, DynArray*);
        if (dynarray == NULL || dynarray->data == NULL) {
            va_end(args);
            va_end(args_len_check);
            return snitch("Invalid input", __LINE__, __FILE__);
        }
        if (dynarray->item_type != item_type || dynarray->item_size != item_size) {
            va_end(args);
            va_end(args_len_check);
            return snitch("Invalid input", __LINE__, __FILE__);
        }
        total_len += dynarray->len;
    }
    va_end(args_len_check);
    total_len += first_dynarray->len;

    // Update the joined dynarray (allocate memory)
    joined->item_type = item_type;
    joined->item_size = item_size;
    joined->len = total_len;
    joined->capacity = total_len > 0 ? total_len * 2: 1; // Double the capacity or 1 if len is 0.
    joined->data = realloc(joined->data, joined->capacity * item_size);
    if (!joined->data) {
        va_end(args);
        return snitch("Memory error", __LINE__, __FILE__);
    }

    // Copy elements (correct va_arg usage)
    Int64 offset = 0;
    // memcpy((Byte*)joined->data + offset * item_size, first_dynarray->data, first_dynarray->len * item_size);
    void* res1 = memmove((Byte*)joined->data + offset * item_size, first_dynarray->data, first_dynarray->len * item_size);
    if (!res1) fatal(snitch("Memory error", __LINE__, __FILE__));

    offset += first_dynarray->len;
    for (Int64 i = 0; i < n - 1; i++) { // n-1 because we already processed the first
        DynArray* dynarray = va_arg(args, DynArray*);
        // memcpy((Byte*)joined->data + offset * item_size, dynarray->data, dynarray->len * item_size);
        void* res2 = memmove((Byte*)joined->data + offset * item_size, dynarray->data, dynarray->len * item_size);
        if (!res2) fatal(snitch("Memory error", __LINE__, __FILE__));

        offset += dynarray->len;
    }
    va_end(args);

    return (Error){.ok = true};
}

// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another array
// For reducing, the acc is any value type
Error dynarray_oneach(DynArray* dynarray, void* acc, Error (*fn)(Int64, Int64, void*, void*)) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (acc == NULL || fn == NULL) return snitch("Invalid input", __LINE__, __FILE__);
    
    Error err;
    for (Int64 i = 0; i < dynarray->len; i++) {
        void* item = (Byte*)dynarray->data + i * dynarray->item_size;
        err = fn(dynarray->len, i, acc, item);  // fn should modify result in place
        if (!err.ok) return err;
    }

    return (Error){ .ok = true };
}

// // TODO - also send length of the array


// Filter function. Iterates over the dynarray and applies the predicate to each element.
// Returns a new array with only thos elements that satisfy the predicate
Error dynarray_filter(DynArray* source, DynArray* filtered, Error (*fn)(Int64, Int64, void*, Bool*)) {
    if (source == NULL || source->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (filtered == NULL || filtered->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (fn == NULL) return snitch("Null input", __LINE__, __FILE__);

    Error err;

    for (Int64 i = 0; i < source->len; i++) {
        Bool res;
        void* item = (Byte*)source->data + i * source->item_size;
        err = fn(source->len, i, item, &res); 
        if (!err.ok) return err;

        if (res) {
            err = dynarray_push(filtered, item, source->item_size);
            if (!err.ok) return err;
        }
    }
    
    return (Error){ .ok = true };
}

Error dynarray_sort(DynArray* dynarray, DynArray* sorted, int (*fn)(const void *, const void *)) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (sorted == NULL || sorted->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (fn == NULL) return snitch("Null input", __LINE__, __FILE__);

    void* res = memmove(sorted->data, dynarray->data, dynarray->len * dynarray->item_size);
    if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));

    sorted->len = dynarray->len;
    qsort(sorted->data, sorted->len, sorted->item_size, fn); // Call qsort directly

    return (Error){ .ok = true };
}

Error dynarray_compare(DynArray* arr1, DynArray* arr2, Error (*cmp)(void*, void*, Bool*), Bool* result) {
    if (arr1 == NULL || arr1->data == NULL || arr2 == NULL || arr2->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (arr1->len != arr2->len || arr1->item_size != arr2->item_size) {
        *result = false;
        return (Error){ .ok = true };
    }
    
    Error err;
    for (Int64 i = 0; i < arr1->len; i++) {
        Bool res;
        void* item1 = (Byte*)arr1->data + i * arr1->item_size;
        void* item2 = (Byte*)arr2->data + i * arr2->item_size;
        err = cmp(item1, item2, &res); if (!err.ok) return err;

        if (!res) {
            *result = false;
            return (Error){ .ok = true };
        }
    }
    *result = true;
    return (Error){ .ok = true };    
}

Error dynarray_find_first(DynArray* dynarray, Int64 from, void* item, Int64* at, Error (*cmp)(void*, void*, Bool*)) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (from < 0 || from >= dynarray->len) return snitch("Out of bounds input", __LINE__, __FILE__);
    if (at == NULL || cmp == NULL) return snitch("Invalid input", __LINE__, __FILE__);
    if( item == NULL) return snitch("Invalid input", __LINE__, __FILE__);

    Error err;
    *at = -1; // Not found
    for (Int64 i = from; i < dynarray->len; i++) {
        Bool res;
        void* curr_item = (Byte*)dynarray->data + i * dynarray->item_size;
        err = cmp(curr_item, item, &res); if (!err.ok) return err;

        if (res) {
            *at = i;
            return (Error){ .ok = true};
        }
    }
    return (Error){ .ok = true};
}

// get overlap with other array - intersects
// Other methods
// to string
// pretty print
// slice - return a new list with elements from a start to an end index
// splice - remove elements from a list and optionally add new elements
// find first
// find all
// includes - check if a list includes a specific element
// some - check if any element in a list satisfies a condition
// every - check if all elements in a list satisfy a condition
// join - join the elements of a list into a string
// reverse - reverse the list