#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef HX_COMPILER_HELEPRS_H
#define HX_COMPILER_HELPERS_H


typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer* create_buffer(size_t initial_capacity) ;
void add_to_buffer(Buffer* buffer, char ch) ;
void close_buffer(Buffer* buffer) ;
void destroy_buffer(Buffer* buffer) ;
bool are_buffers_equal(Buffer* buffer1, Buffer* buffer2);
bool buffer_has(Buffer* buffer, size_t i, char* frag);

// Rest of your code...

// // Use the buffer functions like this:
// Buffer* buffer = create_buffer(16); // Initial capacity of 16
// add_to_buffer(buffer, 'H');
// add_to_buffer(buffer, 'e');
// add_to_buffer(buffer, 'l');
// add_to_buffer(buffer, 'l');
// add_to_buffer(buffer, 'o');
// printf("Buffer: %s\n", buffer->data);
// destroy_buffer(buffer); // Clean up when done

// Define the array structure
typedef struct {
    void** data;     // Array of void pointers (references to items)
    size_t size;     // Current number of items in the array
    size_t capacity; // Maximum capacity of the array
} List;

// Function to create a new dynamic array with an initial capacity
List* list_new(size_t initial_capacity);
// Function to push an item (reference) into the dynamic array
void list_push(List* array, void* item);
// Function to pop an item (reference) from the dynamic array
void* list_pop(List* array);
// Function to get an item (reference) from the dynamic array by index
void* list_get(const List* array, size_t index);
// Function to destroy the dynamic array and free its memory
void list_destroy(List* array);




#endif // HX_COMPILER_HELPERS_H