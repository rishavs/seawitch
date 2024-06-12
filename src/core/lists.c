#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "../seawitch.h"

#define INITIAL_CAPACITY 4

typedef struct {
    int **data;
    int size;
    int capacity;
} List;

List *list_create(int initial_capacity);
{
    List *list = malloc(sizeof(List));
    list->data = malloc(initial_capacity * sizeof(void*));
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

void list_push(List *list, void *element)
{
    if (list->size == list->capacity) {
        // Increase the capacity of the list
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(void*));
    }

    // Add the element to the end of the list
    list->data[list->size++] = element;
}

void *list_pop(List *list)
{
    if (list->size == 0) {
        // The list is empty
        return NULL;
    }

    // Remove the last element from the list
    return list->data[--list->size];
}

List *list_concat(List *list1, List *list2)
{
    List *list3 = list_init(list1->capacity + list2->capacity);

    for (int i = 0; i < list1->size; i++) {
        list_push(list3, list1->data[i]);
    }
    for (int i = 0; i < list2->size; i++) {
        list_push(list3, list2->data[i]);
    }

    return list3;
}

void list_free(List *list);

void list_print(List *list)
{
    printf("[");
    for (int i = 0; i < list->size; i++) {
        printf("%d", list->data[i]);
        if (i < list->size) {
            printf(", ");
        }
    }
    printf("]\n");
}


int main()
{
    List *list = list_create(INITIAL_CAPACITY);

    for (int i = 0; i < 10; i++) {
        list_push(list, i);
    }

    list_print(list);

    for (int i = 0; i < 5; i++) {
        printf("%d ", list_pop(list));
    }
    printf("\n");
    list_print(list);

    list_free(list);

    return 0;
}




int list_pop(List *list)
{
    if (list->size == 0) {
        // The list is empty
        return 0;
    }

    // Remove the last element from the list
    return list->data[--list->size];
}

void list_free(List *list)
{
    free(list->data);
    free(list);
}

// ---------------------------------
// ---------------------------------
// ---------------------------------

// // Initialize a list instance with the given element size and capacity
// void list_init(struct List *list, size_t element_size, size_t capacity)
// {
//     list->data = malloc(capacity * element_size);
//     if (list->data == NULL)
//     {
//         // malloc() failed, exit with an error
//         perror("malloc failed");
//         exit(1);
//     }
//     list->len = 0;
//     list->capacity = capacity;
//     list->element_size = element_size;
// }

// // Resize the list if necessary
// static void list_resize(struct List *list, size_t new_capacity)
// {
//     void *new_data = realloc(list->data, new_capacity * list->element_size);
//     if (new_data == NULL)
//     {
//         // realloc() failed, exit with an error
//         perror("realloc failed");
//         exit(1);
//     }
//     list->data = new_data;
//     list->capacity = new_capacity;
// }

// // Append an element to the list
// void list_append(struct List *list, void *element)
// {
//     // Resize the dynamic array if necessary
//     if (list->len + 1 > list->capacity)
//     {
//         list_resize(list, list->capacity * 2);
//     }

//     // Append the element to the end of the list
//     memcpy(&list->data + list->len * list->element_size, element, list->element_size);
//     list->len++;
// }

// // Free the memory allocated by the list
// void list_free(struct List *list)
// {
//     free(list->data);
// }


