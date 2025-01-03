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


// Run a function On Each item. Outcome of the operations are accumulated in the acc
// Can be used for both mapping and reducing. 
// For mapping, the acc is another array
// For reducing, the acc is any value type
Error dynarray_oneach(DynArray* dynarray, void* acc, Error (*fn)(Int64, Int64, void*, void*)) {
    if (dynarray == NULL || dynarray->data == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (acc == NULL || fn == NULL) return snitch("Invalid input", __LINE__, __FILE__);

    for (Int64 i = 0; i < dynarray->len; i++) {
        void* item = (Byte*)dynarray->data + i * dynarray->item_size;
        Error err = fn(dynarray->len, i, acc, item);  // fn should modify result in place
        if (!err.ok) return err;
    }

    return (Error){ .ok = true };
}

// DynArray *dynarray_join(Int64 n, ...) {
//     if (n <= 0) return print_error_return_null(__FILE__, __LINE__);

//     va_list args;
//     va_start(args, n);

//     DynArray* first_dynarray = va_arg(args, DynArray*);
//     if (!first_dynarray) {
//         va_end(args);
//         return print_error_return_null(__FILE__, __LINE__);
//     }

//     Types item_type = first_dynarray->item_type;
//     Int64 item_size = first_dynarray->item_size;

//     // Check type and size consistency
//     for (Int64 i = 0; i < n ; i++) {
//         DynArray *dynarray = va_arg(args, DynArray*);
//         if (!dynarray || dynarray->item_type != item_type || dynarray->item_size != item_size) {
//             va_end(args);
//             return print_error_return_null(__FILE__, __LINE__);
//         }
//     }
//     va_end(args);

//     // Calculate total length
//     va_start(args, n);
//     Int64 total_len = 0;
//     for (Int64 i = 0; i < n; i++) {
//         DynArray *dynarray = va_arg(args, DynArray *);
//         if (dynarray) {
//             total_len += dynarray->len;
//         }
//     }
//     va_end(args);

//     // Create joined dynarray
//     DynArray *joined_dynarray = dynarray_create(item_type, item_size, total_len * 2); // Double the capacity
//     joined_dynarray->len = total_len; // Set the correct length

//     // Copy elements
//     va_start(args, n);
//     Int64 offset = 0;
//     for (Int64 i = 0; i < n; i++) {
//         DynArray *dynarray = va_arg(args, DynArray *);
//         if (dynarray) {
//             memcpy(
//                 joined_dynarray->data + offset * item_size, 
//                 dynarray->data, 
//                 dynarray->len * item_size
//             );
//             if (!dynarray->data) return print_error_return_null(__FILE__, __LINE__);
//             offset += dynarray->len;
//         }
//     }
//     va_end(args);

//     return joined_dynarray;
// }

// // TODO - also send length of the array


// // Filter function. Iterates over the dynarray and applies the predicate to each element.
// // Returns a new array with only thos elements that satisfy the predicate
// DynArray* dynarray_filter(DynArray* dynarray, Bool (*predicate)(void*)) {
//     if (!dynarray || !predicate) return print_error_return_null(__FILE__, __LINE__);
    
//     DynArray* result = dynarray_create(dynarray->item_type, dynarray->item_size, dynarray->len);
//     if (!result) return print_error_return_null(__FILE__, __LINE__);
    
//     for (Int64 i = 0; i < dynarray->len; i++) {
//         void* item = (Byte*)dynarray->data + i * dynarray->item_size;
//         if (predicate(item)) {
//             dynarray_push(result, item);
//         }
//     }
    
//     return result;
// }

// DynArray* dynarray_sort(DynArray* dynarray, int (*cmp)(const void *, const void *)) { // int (*cmp)
//     if (!dynarray || !cmp) return print_error_return_null(__FILE__, __LINE__);

//     DynArray* result = dynarray_create(dynarray->item_type, dynarray->item_size, dynarray->len);
//     if (!result) return print_error_return_null(__FILE__, __LINE__);

//     memcpy(result->data, dynarray->data, dynarray->len * dynarray->item_size);
//     if (!result->data) return print_error_return_null(__FILE__, __LINE__);

//     result->len = dynarray->len;
//     qsort(result->data, result->len, result->item_size, cmp); // Call qsort directly
//     if (!result->data) return print_error_return_null(__FILE__, __LINE__);

//     return result;
// }

// Bool dynarray_compare(DynArray* dynarray1, DynArray* dynarray2, Bool (*cmp)(void*, void*)) {
//     if (!dynarray1 || !dynarray2 || !cmp) return print_error_return_false(__FILE__, __LINE__);

//     if (dynarray1->len != dynarray2->len) return false;

//     for (Int64 i = 0; i < dynarray1->len; i++) {
//         void* item1 = (Byte*)dynarray1->data + i * dynarray1->item_size;
//         void* item2 = (Byte*)dynarray2->data + i * dynarray2->item_size;
//         if (!cmp(item1, item2)) return false;
//     }

//     return true;
// }

// // Compare two dynamic arrays
// Bool dynarray_compare(DynArray* arr1, DynArray* arr2) {
//     if (arr1 == NULL || arr2 == NULL) return false;
//     if (arr1->len != arr2->len) return false;
//     if (arr1->item_size != arr2->item_size) return false;

//     return memcmp(arr1->data, arr2->data, arr1->len * arr1->item_size) == 0;
// }

// Bool dynarray_has_subarray(DynArray* dynarray, Int64 pos, DynArray* subarray) {
//     if (!dynarray || !subarray) return print_error_return_false(__FILE__, __LINE__);

//     if (pos >= dynarray->len) return false;
//     if (subarray->len > dynarray->len - pos) return false;

//     for (Int64 i = 0; i < subarray->len; i++) {
//         void* item1 = (Byte*)dynarray->data + (pos + i) * dynarray->item_size;
//         void* item2 = (Byte*)subarray->data + i * subarray->item_size;
//         if (memcmp(item1, item2, dynarray->item_size) != 0) return false;
//     }

//     return true;
// }

// // Find the index of an item in the dynarray
// // Uses a comparison function to compare items
// Bool dynarray_find(DynArray* dynarray, Int64* out,  void* item, Bool (*cmp)(void*, void*)) {
//     if (!dynarray || !out || !item || !cmp) return print_error_return_false(__FILE__, __LINE__);

//     for (Int64 i = 0; i < dynarray->len; i++) {
//         void* current = (Byte*)dynarray->data + i * dynarray->item_size;
//         if (cmp(current, item)) {
//             *out = i;
//             return true;
//         }
//     }

//     return false;
// }

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