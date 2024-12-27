#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "errors.h"
#include "pixel.h"

// FNV-1a hash function
static inline uint64_t fnv1a_hash(const char *key) {
    uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    while (*key) {
        hash ^= (unsigned char)(*key);
        hash *= 1099511628211ULL; // FNV prime
        key++;
    }
    return hash;
}

// Macro to define a generic hashmap
#define DEFINE_DICT(value_type, name)                            \
    typedef struct {                                               \
        char *key;                                                 \
        value_type value;                                          \
    } Entry_of_##name##_t;                                         \
                                                                   \
    typedef struct {                                               \
        Entry_of_##name##_t *data;                                 \
        size_t count;                                              \
        size_t capacity;                                           \
    } Dict_of_##name##_t;                                        \
                                                                   \
    static inline void Dict_of_##name##_do_init(Dict_of_##name##_t *map, size_t initial_capacity) { \
        map->count = 0;                                            \
        map->capacity = initial_capacity ? initial_capacity : DICT_INITIAL_CAPACITY; \
        map->data = calloc(map->capacity, sizeof(Entry_of_##name##_t)); \
        if (!map->data) memory_allocation_failure(__FILE__, __LINE__); \
    }                                                              \
                                                                   \
    static inline void Dict_of_##name##_do_free(Dict_of_##name##_t *map) { \
        for (size_t i = 0; i < map->capacity; i++) {               \
            if (map->data[i].key) {                                \
                free(map->data[i].key);                            \
            }                                                      \
        }                                                          \
        free(map->data);                                           \
    }                                                              \
                                                                   \
    static inline void Dict_of_##name##_do_rehash(Dict_of_##name##_t *map) { \
        size_t new_capacity = map->capacity > 0 ? map->capacity * 2 : DICT_INITIAL_CAPACITY; \
        Entry_of_##name##_t *new_data = calloc(new_capacity, sizeof(Entry_of_##name##_t)); \
        if (!new_data) {                                           \
            free(map->data);                                       \
            memory_allocation_failure(__FILE__, __LINE__);   \
        }                                                          \
                                                                   \
        for (size_t i = 0; i < map->capacity; i++) {               \
            if (map->data[i].key) {                                \
                uint64_t hash = fnv1a_hash(map->data[i].key);      \
                size_t idx = hash % new_capacity;                  \
                while (new_data[idx].key) {                        \
                    idx = (idx + 1) % new_capacity;                \
                }                                                  \
                new_data[idx] = map->data[i];                      \
            }                                                      \
        }                                                          \
        free(map->data);                                           \
        map->data = new_data;                                      \
        map->capacity = new_capacity;                              \
    }                                                              \
                                                                   \
    static inline void Dict_of_##name##_do_set(Dict_of_##name##_t *map, const char *key, value_type value) { \
        if (map->count >= map->capacity / 2) {                     \
            Dict_of_##name##_do_rehash(map);                     \
        }                                                          \
        uint64_t hash = fnv1a_hash(key);                           \
        size_t idx = hash % map->capacity;                         \
        while (map->data[idx].key) {                               \
            if (strcmp(map->data[idx].key, key) == 0) {            \
                map->data[idx].value = value;                      \
                return;                                            \
            }                                                      \
            idx = (idx + 1) % map->capacity;                       \
        }                                                          \
        map->data[idx].key = strdup(key);                          \
        if (!map->data[idx].key) memory_allocation_failure(__FILE__, __LINE__); \
        map->data[idx].value = value;                              \
        map->count++;                                              \
    }                                                              \
                                                                   \
    static inline int Dict_of_##name##_do_get(Dict_of_##name##_t *map, const char *key, value_type *out_value) { \
        if (map->capacity == 0) return 0;                          \
        uint64_t hash = fnv1a_hash(key);                           \
        size_t idx = hash % map->capacity;                         \
        while (map->data[idx].key) {                               \
            if (strcmp(map->data[idx].key, key) == 0) {            \
                *out_value = map->data[idx].value;                 \
                return 1;                                          \
            }                                                      \
            idx = (idx + 1) % map->capacity;                       \
        }                                                          \
        return 0;                                                  \
    }                                                              \
                                                                   \
    static inline int Dict_of_##name##_do_remove(Dict_of_##name##_t *map, const char *key) { \
        if (map->capacity == 0) return 0;                          \
        uint64_t hash = fnv1a_hash(key);                           \
        size_t idx = hash % map->capacity;                         \
        while (map->data[idx].key) {                               \
            if (strcmp(map->data[idx].key, key) == 0) {            \
                free(map->data[idx].key);                          \
                map->data[idx].key = NULL;                         \
                map->count--;                                      \
                return 1;                                          \
            }                                                      \
            idx = (idx + 1) % map->capacity;                       \
        }                                                          \
        return 0;                                                  \
    }
#endif // DICT_H