#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seawitch.h"

#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 8
#endif

typedef struct {
    Gen_ref     items; //void*
    Types       item_type;
    Int64       item_size;
    Int64       len;
    Int64       capacity;
} List; // list owns the data

List* list_create(Types item_type, Int64 item_size, Int64 initial_capacity) {
    if (item_size == 0) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    List *list = calloc(1, sizeof(List));
    if (list == NULL) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    list->item_type = item_type;
    list->item_size = item_size;
    list->len = 0;
    list->capacity = initial_capacity ? initial_capacity : INITIAL_CAPACITY;
    
    list->items = calloc(list->capacity, list->item_size);
    if (list->items == NULL) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    return list;
}

Int64 list_push(List* list, Gen_ref item) {
    if (!list || !item ) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    // Resize the list if necessary
    if (list->len == list->capacity) {                                                                                                    
        if (list->capacity < SIZE_MAX / 2) list->capacity *= 2;                                                                          
        else if (list->capacity >= SIZE_MAX / 2 && list->capacity < SIZE_MAX) list->capacity = SIZE_MAX;                                                                    
        else return -1;                                                                                         
                                                                                                         
        list->items = realloc(list->items, list->capacity * list->item_size);                                  
        if (!list->items) return -1;                                                                                    
    } 

    // Add the element to the end of the list. Save the items as Bytes, which are the smallest data type
    memcpy((Byte*)list->items + list->len * list->item_size, item, list->item_size);
    list->len++;

    return list->len; // we return the index, instead of index - 1, as our list is 1-indexed
}

Int64 list_pop(List* list, Gen_ref out) {
    if (!list || !out) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    // If the list is empty
    if (list->len == 0) return 0;

    // Remove the last element from the list
    memcpy(out, (Byte*)list->items + (list->len - 1) * list->item_size, list->item_size);  
    list->len--;

    return list->len + 1;
}

Int64 list_get(List* list, Int64 index, Gen_ref out) {
    if (!list || !out) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    // If the index is out of bounds
    if (index < 1 || index >= list->len) return -1;

    Int64 cindex = index - 1;
    memcpy(out, (Byte *)list->items + cindex * list->item_size, list->item_size);  // remember, our list is 1-indexed

    return index;    
}

Int64 list_set (List* list, Int64 index, Gen_ref item) {
    if (!list || !item) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    // If the index is out of bounds
    if (index < 1 || index >= list->len) return -1;

    Int64 cindex = index - 1;
    memcpy((Byte*)list->items + cindex * list->item_size, item, list->item_size); // remember, our list is 1-indexed

    return index;
}

List* list_slice(List* list, Int64 start, Int64 end) {
    if (!list) return NULL;

    // If the start index is out of bounds
    if (start < 1 || start > list->len || end < 1 || end > list->len || end < start) return NULL;

    // Create a new list
    Int64 new_len = end - start + 1;
    List* new_list = list_create(list->item_type, list->item_size, new_len * 2); // Double the capacity
    new_list->len = new_len;

    // Copy the elements from the original list to the new list
    for (int64 i = 0; i <= new_list->len - 1; i++) {
        list_get(list, i + start, (Byte *)new_list->items + i * new_list->item_size);
    }
    return new_list;
}

List *list_join(Int64 n, ...) {
    if (n <= 0) return NULL; // Handle n <= 0 case

    va_list args;
    va_start(args, n);

    List *first_list = va_arg(args, List *);
    if (!first_list) {
        va_end(args);
        return NULL;
    }

    Types item_type = first_list->item_type;
    Int64 item_size = first_list->item_size;

    // Check type and size consistency
    for (Int64 i = 1; i < n; i++) {
        List *list = va_arg(args, List *);
        if (!list || list->item_type != item_type || list->item_size != item_size) {
            va_end(args);
            return NULL;
        }
    }
    va_end(args); // Important: End the first va_list

    // Calculate total length
    va_start(args, n); // Restart va_list
    Int64 total_len = 0;
    for (Int64 i = 0; i < n; i++) {
        List *list = va_arg(args, List *);
        if (list) {
            total_len += list->len;
        }
    }
    va_end(args); // Important: End the second va_list

    // Create joined list
    List *joined_list = list_create(item_type, item_size, total_len * 2); // Double the capacity
    joined_list->len = total_len; // Set the correct length

    // Copy elements
    va_start(args, n); // Restart va_list AGAIN
    Int64 current_index = 1; // Start from 1 since lists are 1-indexed

    for (Int64 i = 0; i < n; i++) {
        List *current_list = va_arg(args, List *);
        if (current_list) {
            for (Int64 j = 1; j <= current_list->len; j++) { // Use 1-based indexing here
                list_get(current_list, j, (Byte *)joined_list->items + (current_index - 1) * joined_list->item_size);
                current_index++;
            }
        }
    }
    va_end(args);

    return joined_list;
}
// // print64_t the list. takes a function as input to print64_t each item
// void list_print(List* list, void (*print_item)(void*)) {
//     if (list == NULL) return NULL;

//     printf("[");
//     if (list->len > 0) {
//         print_item(list->data[0]);
//         for (int64_t i = 1; i < list->len; i++) {
//             printf(", "); // Add comma between elements
//             print_item(list->data[i]);
//         }
//     }
//     printf("]\n");
// }

// Other methods
// each - iterate over each element in a list. Alternative to for loop
// map - apply a function to each element in a list and return a new list
// reduce - apply a function to each element in a list and return a single value
// filter - return a new list with elements that satisfy a condition
// find - return the first element that satisfies a condition
// sort - sort the list
// reverse - reverse the list
// slice - return a new list with elements from a start to an end index
// splice - remove elements from a list and optionally add new elements
// indexOf - find the index of an element in a list
// lastIndexOf - find the last index of an element in a list
// includes - check if a list includes a specific element
// some - check if any element in a list satisfies a condition
// every - check if all elements in a list satisfy a condition
// join - join the elements of a list into a string

