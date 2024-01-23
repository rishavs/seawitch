#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define the list structure
typedef struct {
    void** data;      // list of void pointers (references to items)
    size_t size;      // Current number of items in the list
    size_t capacity;  // Maximum capacity of the list
} List;

// Function to create a new dynamic list with an initial capacity
List* list_new(size_t initial_capacity) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    list->data = (void**)malloc(initial_capacity * sizeof(void*));
    if (list->data == NULL) {
        perror("Memory allocation failed");
        free(list);
        exit(1);
    }

    list->size = 0;
    list->capacity = initial_capacity;

    return list;
}

// Function to destroy the dynamic list and free its memory
void list_destroy(List* list, void (*destructor)(void*)) {
    if (list == NULL) {
        return;
    }

    if (destructor != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            destructor(list->data[i]);
        }
    }

    free(list->data);
    free(list);
}
// Function to push an item (reference) into the dynamic list
List* list_push(List* list, void* item) {
    if (list->size >= list->capacity) {
        // Double the capacity if the list is full
        list->capacity *= 2;
        void** new_data = (void**)realloc(list->data, list->capacity * sizeof(void*));
        if (new_data == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
        list->data = new_data;
    }

    list->data[list->size] = item;
    list->size++;

    return list;  // Return the updated list
}

// Function to pop the last item from the dynamic list and return it
void* list_pop(List* list) {
    if (list->size == 0) {
        // list is empty
        return NULL;
    }

    void* item = list->data[list->size - 1];
    list->size--;

    return item;
}

// Function to get an item (reference) from the dynamic list by index
void* list_get(List* list, size_t index) {
    if (index < list->size) {
        return list->data[index];
    }
    perror("Index Out of Bounds");
    return NULL; // Index out of bounds
}

// Function to set an item's value
List* list_set(List* list, size_t index, void* item) {
    if (index < list->size) {
        list->data[index] = item;
    } else {
        perror("Index Out of Bounds");
    }

    return list;  // Return the updated list
}


// int main() {
//     // Define the size of each element (e.g., sizeof(MyStruct))
//     size_t element_size = sizeof(MyStruct);

//     // Create a list
//     List* myList = list_new(10, element_size);

//     // Create a sample struct
//     MyStruct struct1 = {1, "Alice"};

//     // Add the struct to the list
//     list_push(myList, &struct1);

//     // Retrieve and print the struct from the list
//     MyStruct* retrievedStruct = (MyStruct*)list_get(myList, 1);
//     printf("ID: %d, Name: %s\n", retrievedStruct->id, retrievedStruct->name);

//     // Destroy the list
//     list_destroy(myList);

//     return 0;
// }
