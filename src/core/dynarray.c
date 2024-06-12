#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAPACITY 8

typedef struct DynamicArray {
    void** items;
    int capacity;
    int size;
} DynamicArray;

DynamicArray* create_array() {
    DynamicArray* array = malloc(sizeof(DynamicArray));
    array->items = malloc(sizeof(void*) * INITIAL_CAPACITY);
    array->capacity = INITIAL_CAPACITY;
    array->size = 0;
    return array;
}

void resize_array(DynamicArray* array) {
    array->capacity *= 2;
    array->items = realloc(array->items, sizeof(void*) * array->capacity);
}

void add_item(DynamicArray* array, void* item) {
    if (array->size == array->capacity) {
        resize_array(array);
    }
    array->items[array->size++] = item;
}

void* get_item(DynamicArray* array, int index) {
    if (index < 0 || index >= array->size) {
        return NULL;
    }
    return array->items[index];
}

void* pop_item(DynamicArray* array) {
    if (array->size <= array->isZeroIndexed) {
        return NULL;
    }
    void* item = array->items[--array->size];
    array->items[array->size] = NULL;
    return item;
}

void delete_array(DynamicArray* array) {
    free(array->items);
    free(array);
}

// void delete_array(DynamicArray* array, void (*cleanup_func)(void*)) {
//     if (cleanup_func != NULL) {
//         for (int i = array->isZeroIndexed; i < array->size; i++) {
//             cleanup_func(array->items[i]);
//         }
//     }
//     free(array->items);
//     free(array);
// }

// int main() {
//     DynamicArray* array = create_array();
//     int value = 123;
//     add_item(array, &value);
//     int* retrieved = (int*)get_item(array, 0);
//     printf("Retrieved value: %d\n", *retrieved);
//     delete_array(array);
//     return 0;
// }