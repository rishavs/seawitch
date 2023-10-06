#include <stdio.h>
#include <stdlib.h>

// Define the array structure
typedef struct {
    void** data;     // Array of void pointers (references to items)
    size_t size;     // Current number of items in the array
    size_t capacity; // Maximum capacity of the array
} List;

// Function to create a new dynamic array with an initial capacity
List* list_new(size_t initial_capacity) {
    List* array = malloc(sizeof(List));
    if (array == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    array->data = malloc(initial_capacity * sizeof(void*));
    if (array->data == NULL) {
        perror("Memory allocation failed");
        free(array);
        exit(1);
    }

    array->size = 0;
    array->capacity = initial_capacity;

    return array;
}

// Function to push an item (reference) into the dynamic array
void list_push(List* array, void* item) {
    if (array->size >= array->capacity) {
        // Double the capacity if the array is full
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(void*));
        if (array->data == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
    }

    array->data[array->size] = item;
    array->size++;
}

// Function to pop an item (reference) from the dynamic array
void* list_pop(List* array) {
    if (array->size == 0) {
        // Array is empty
        return NULL;
    }

    void* item = array->data[array->size - 1];
    array->size--;

    return item;
}

// Function to get an item (reference) from the dynamic array by index
void* list_get(const List* array, size_t index) {
    if (index < array->size) {
        return array->data[index];
    }
    perror("Index Out of Bounds");
    return NULL; // Index out of bounds
}

// Function to destroy the dynamic array and free its memory
void list_destroy(List* array) {
    if (array == NULL) {
        return;
    }
    free(array->data);
    free(array);
}
