#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "dynarray.h"
#include "dynhashmap.h"
#include "seawitch.h"

#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR 0.75
#define DICT_MAX_KEY_LENGTH 256

typedef struct Item {
    FxString key;

    Int64   at; // index of the data in the data array

    Int64  pdistance; // New: Probe distance array
    Int64  hvalue; // New: Hash values array
} Item;

typedef struct Record {
    Item*  items;
    Byte*   data; //data is stored as bytes

    Types   item_type;
    Int64   item_size;
    Int64   len;
    Int64   capacity;
} Record;


// FNV-1a hash function
static inline Int64 hash(const char* str) {
    Int64 hash = 0xcbf29ce484222325LL;
    unsigned char* s = (unsigned char*)str;
    
    while (*s) {
        hash ^= *(Int64*)s;
        hash *= 0x100000001b3LL;
        s += 4;
    }
    
    hash ^= hash >> 32; // Additional mixing
    return hash;
}


Record* record_create(Types item_type, Int64 item_size, Int64 initial_capacity) {
    Record *record = calloc(1, sizeof(Record));
    if (record ==  NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    record->item_type = item_type;
    record->item_size = item_size;
    record->len = 0;
    record->capacity = initial_capacity ? initial_capacity : DICT_INITIAL_CAPACITY;
    
    record->items = calloc(record->capacity, sizeof(Item));
    if (record->items == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    record->data = calloc(record->capacity, item_size);
    if (record->data == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    return record;
}

Int64 record_find_key(Record* record, FxString key) {
    if (!record || !key) memory_allocation_failure(NULL, NULL, NULL, __FILE__, __LINE__);

    Int64 base_index = hash(key.data) % record->capacity;
    Int64 i = 0;
    Int64 index;
    
    while (i < record->capacity) {
        // Quadratic probing formula: (base + i + i*i) % capacity
        index = (base_index + i*i) % record->capacity;
        if (strcmp(record->items[index].key.data, key) == 0) return index;
        i++;
    }
    return -1;
}

Error* record_set (Record* record, FxString key, void* value) {
    if (!record || !key.data || !value) fatal(snitch("Null input", __LINE__, __FILE__));

    Error* err;
    err = record_resize(record);
    if (!err->ok) return err;

    Int64 index = record_find_key(record, key);
    if (index != -1) {
        void* res = memmove((Byte*)record->data + index * record->item_size, value, record->item_size);
        if (!res) fatal(snitch("Memory error", __LINE__, __FILE__));
        return (Error){.ok = true};
    }

    Int64 base_index = hash(key.data) % record->capacity;
    Int64 i = 0;
    Int64 index;

    while (i < record->capacity) {
        // Quadratic probing formula: (base + i + i*i) % capacity
        index = (base_index + i*i) % record->capacity;
        if (record->items[index].key.data == NULL) break; 
        i++;
    }

    if (i == record->capacity) return snitch("Record is full", __LINE__, __FILE__);

    record->items[index].key = key;
    record->items[index].at = index;
    record->items[index].pdistance = i;
    record->items[index].hvalue = hash(key.data);
    
}

void record_resize(Error* err, Record* record) {
    if (!record) fatal(snitch("Null input", __LINE__, __FILE__));

    if ((float)record->len / record->capacity < DICT_LOAD_FACTOR) {
        *err = (Error){.ok = true};
        return;
    }

    Int64 new_capacity = record->capacity * 2;
    Record* new_record = record_create(record->item_type, record->item_size, new_capacity);
    
    for (Int64 i = 0, transferred = 0; i < record->capacity; i++) {
        if (transferred >= record->len) break; 
        if (record->items[i] && record->items[i].key.data) {
            Int64 index = hash(record->items[i].key.data) % new_capacity;
            Error err = record_set(new_record, new_record->keys[i].data, (Byte*)new_record->data + i * hmap->item_size);
            if (!err.ok) return err;

            transferred++;
        }
    }
    
    // Transfer ownership
    record->items = new_record->items;
    record->capacity = new_record->capacity;

    *err = (Error){.ok = true};
}
