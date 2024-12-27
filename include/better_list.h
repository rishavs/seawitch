#ifndef SEAWITCH_LIST_H
#define SEAWITCH_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h> // For SIZE_MAX
#include <stdarg.h> // For variadic arguments

// Assuming these headers have header guards
#include "seawitch.h"

#ifndef INITIAL_CAPACITY
#define INITIAL_CAPACITY 8
#endif

// Macro to define a generic dynamic list type
#define DEFINE_LIST(type, name)                                                                               \
                                                                                                              \
    typedef struct                                                                                            \
    {                                                                                                         \
        type *data;                                                                                           \
        Int64 count;                                                                                          \
        Int64 capacity;                                                                                       \
    } List_of_##name##_t;                                                                                     \
                                                                                                              \
    static inline List_of_##name##_t *list_of_##name##_do_init(Int64 initial_capacity)                        \
    {                                                                                                         \
        List_of_##name##_t *list = calloc(1, sizeof(List_of_##name##_t));                                        \
        if (!list)                                                                                            \
            return NULL;                                                                                      \
                                                                                                              \
        list->data = NULL;                                                                                    \
        list->count = 0;                                                                                      \
        list->capacity = initial_capacity ? initial_capacity : INITIAL_CAPACITY;                              \
        list->data = calloc(list->capacity, sizeof(type));                                                    \
        if (!list->data)                                                                                      \
            return NULL;                                                                                      \
                                                                                                              \
        return list;                                                                                          \
    }                                                                                                         \
                                                                                                              \
    static inline void list_of_##name##_do_free(List_of_##name##_t *list)                                     \
    {                                                                                                         \
        if (list)                                                                                             \
        {                                                                                                     \
            free(list->data);                                                                                 \
            free(list);                                                                                       \
        }                                                                                                     \
    }                                                                                                         \
                                                                                                              \
    static inline Int64 list_of_##name##_do_push(List_of_##name##_t *list, type item)                         \
    {                                                                                                         \
        if (!list)                                                                                            \
            return -1;                                                                                        \
        if (list->count == list->capacity)                                                                    \
        {                                                                                                     \
            if (list->capacity < SIZE_MAX / 2)                                                                \
            {                                                                                                 \
                list->capacity *= 2;                                                                          \
            }                                                                                                 \
            else if (list->capacity >= SIZE_MAX / 2 && list->capacity < SIZE_MAX)                             \
            {                                                                                                 \
                list->capacity = SIZE_MAX;                                                                    \
            }                                                                                                 \
            else                                                                                              \
            {                                                                                                 \
                return -1;                                                                                    \
            }                                                                                                 \
                                                                                                              \
            list->data = realloc(list->data, list->capacity * sizeof(type));                                  \
            if (!list->data)                                                                                  \
                return -1;                                                                                    \
        }                                                                                                     \
        list->data[list->count++] = item;                                                                     \
        return list->count;                                                                                   \
    }                                                                                                         \
                                                                                                              \
    static inline bool list_of_##name##_do_pop(List_of_##name##_t *list, type *out)                           \
    {                                                                                                         \
        if (!list || list->count == 0)                                                                        \
            return false;                                                                                     \
        list->count -= 1;                                                                                     \
        *out = list->data[list->count];                                                                       \
        return true;                                                                                          \
    }                                                                                                         \
                                                                                                              \
    static inline bool list_of_##name##_do_set(List_of_##name##_t *list, Int64 index, type item)              \
    {                                                                                                         \
        if (!list || index < 0 || index >= list->count)                                                       \
            return false;                                                                                     \
        list->data[index] = item;                                                                             \
        return true;                                                                                          \
    }                                                                                                         \
                                                                                                              \
    static inline type *list_of_##name##_do_get(List_of_##name##_t *list, Int64 index)                        \
    {                                                                                                         \
        if (!list || index < 0 || index >= list->count)                                                       \
            return NULL;                                                                                      \
        return &list->data[index];                                                                            \
    }                                                                                                         \
                                                                                                              \
    static inline Int64 list_of_##name##_do_count(List_of_##name##_t *list)                                   \
    {                                                                                                         \
        return list ? list->count : 0;                                                                        \
    }                                                                                                         \
                                                                                                              \
    static inline List_of_##name##_t *list_of_##name##_do_join(Int64 n, ...)                                  \
    {                                                                                                         \
        if (n <= 0)                                                                                           \
        {                                                                                                     \
            return NULL;                                                                                      \
        }                                                                                                     \
                                                                                                              \
        va_list args;                                                                                         \
        va_start(args, n);                                                                                    \
                                                                                                              \
        Int64 total_length = 0;                                                                               \
        for (Int64 i = 0; i < n; i++)                                                                         \
        {                                                                                                     \
            List_of_##name##_t *current_list = va_arg(args, List_of_##name##_t *);                            \
            if (!current_list)                                                                                \
            {                                                                                                 \
                va_end(args);                                                                                 \
                return NULL;                                                                                  \
            }                                                                                                 \
            total_length += current_list->count;                                                              \
        }                                                                                                     \
        va_end(args);                                                                                         \
                                                                                                              \
        List_of_##name##_t *joined_list = list_of_##name##_do_init(total_length);                             \
        if (!joined_list)                                                                                     \
        {                                                                                                     \
            return NULL;                                                                                      \
        }                                                                                                     \
                                                                                                              \
        va_start(args, n);                                                                                    \
        for (Int64 i = 0; i < n; i++)                                                                         \
        {                                                                                                     \
            List_of_##name##_t *current_list = va_arg(args, List_of_##name##_t *);                            \
            if (current_list)                                                                                 \
            {                                                                                                 \
                for (Int64 j = 0; j < current_list->count; j++)                                               \
                {                                                                                             \
                    if (list_of_##name##_do_push(joined_list, current_list->data[j]) == -1)                   \
                    {                                                                                         \
                        list_of_##name##_do_free(joined_list);                                                \
                        va_end(args);                                                                         \
                        return NULL;                                                                          \
                    }                                                                                         \
                }                                                                                             \
            }                                                                                                 \
        }                                                                                                     \
        va_end(args);                                                                                         \
        return joined_list;                                                                                   \
    }                                                                                                         \
                                                                                                              \
    static inline String *list_of_##name##_do_to_string(List_of_##name##_t *list, String *(*to_string)(type)) \
    {                                                                                                         \
        if (!list || !to_string)                                                                              \
            return NULL;                                                                                      \
        String *result = string_init("[");                                                                    \
        if (!result)                                                                                          \
            return NULL;                                                                                      \
        for (Int64 i = 0; i < list->count; i++)                                                               \
        {                                                                                                     \
            String item_str = to_string(list->data[i]);                                                       \
            if (!item_str)                                                                                    \
            {                                                                                                 \
                string_free(result);                                                                          \
                return NULL;                                                                                  \
            }                                                                                                 \
            String temp = string_concat(result, item_str);                                                    \
            string_free(item_str);                                                                            \
            string_free(result);                                                                              \
            if (!temp)                                                                                        \
                return NULL;                                                                                  \
            result = temp;                                                                                    \
            if (i < list->count - 1)                                                                          \
            {                                                                                                 \
                temp = string_concat(result, string_init(", "));                                              \
                string_free(result);                                                                          \
                if (!temp)                                                                                    \
                    return NULL;                                                                              \
                result = temp;                                                                                \
            }                                                                                                 \
        }                                                                                                     \
        String temp = string_concat(result, string_init("]"));                                                \
        string_free(result);                                                                                  \
        if (!temp)                                                                                            \
            return NULL;                                                                                      \
        result = temp;                                                                                        \
        return result;                                                                                        \
    }

#endif // SEAWITCH_LIST_H

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
