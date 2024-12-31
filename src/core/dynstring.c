#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "errors.h"
#include "seawitch.h"

// Create a new dynamic string
Outcome dynstring_do_create(DynString* str, char* data) {
    if (str == NULL || data == NULL) return OUT_NULL_INPUT;
    
    size_t str_size = strlen(data);
    if (str_size > INT_MAX) return OUT_INTEGER_OVERFLOW;
    Int64 len = (Int64)str_size;

    str->len = len;
    str->capacity = str->len + 1;
    str->data = calloc(str->capacity, sizeof(char));
    if (str->data == NULL) return OUT_MEMORY_ERROR;

    memcpy(str->data, data, len);

    str->data[len] = '\0'; 

    return OUT_OK;
}

// append a fixed-string or char* to a string
Outcome dynstring_do_push_cstr(DynString* src, char* data) {
     if (src == NULL || data == NULL) return OUT_NULL_INPUT;

    size_t str_size = strlen(data);
    if (str_size > INT64_MAX) return OUT_INTEGER_OVERFLOW;
    Int64 len = (Int64)str_size;

    char* temp = realloc(src->data, src->len + len + 1);
    if (temp == NULL) return OUT_MEMORY_ERROR;
    src->data = temp;

    memcpy(src->data + src->len, data, len);
    src->len += len;
    src->data[src->len] = '\0'; // Explicit null termination

    return OUT_OK;
}

// Get a substring from a string, given a start and end position
Outcome dynstring_do_slice (DynString* src, DynString* result, Int64 start, Int64 end) {
    if (src == NULL || result == NULL ) return OUT_NULL_INPUT;

    if (    
        start < 0 || start >= src->len || 
        end < start || end >= src->len
    ) return OUT_INVALID_INPUT;

    result->len = end - start + 1;
    result->capacity = result->len + 1;
    result->data = calloc(result->capacity, sizeof(char));
    if (result->data == NULL) return OUT_MEMORY_ERROR;

    memcpy(result->data, src->data + start, result->len);

    result->data[result->len] = '\0';
    
    return OUT_OK;
}

Outcome dynstring_do_join(DynString* result, Int64 n, ...) {
    if (result == NULL) return OUT_NULL_INPUT;
    if (n <= 0) return OUT_INVALID_INPUT;

    va_list args;
    va_start(args, n);

    // Calculate total length
    Int64 total_len = 0;
    for (Int64 i = 0; i < n; i++) {
        DynString* str = va_arg(args, DynString*);
        if (str == NULL) {
            va_end(args);
            return OUT_NULL_INPUT;   // One of the input strings is not formed
        }
        total_len += str->len;
    }
    va_end(args);

    // allocate the result string
    result->len = total_len;
    result->capacity = total_len + 1;
    result->data = calloc(result->capacity, sizeof(char));
    if (result->data == NULL) return OUT_MEMORY_ERROR;

    // Copy the data from each input string into the result string
    va_start(args, n);
    Int64 offset = 0;
    for (Int64 i = 0; i < n; i++) {
        DynString* str = va_arg(args, DynString*);
        memcpy(result->data + offset, str->data, str->len);
        offset += str->len;
    }
    va_end(args);

    result->data[result->len] = '\0'; // Explicit null termination

    return OUT_OK;
}

// Compare two strings
Outcome dynstring_do_compare(DynString* str1, DynString* str2) {
    if (str1 == NULL || str2 == NULL) return OUT_NULL_INPUT;
    if (str1->len != str2->len) return OUT_OK_AND_FALSE;

    Bool res = strncmp(str1->data, str2->data, str1->len) == 0;

    return res ? OUT_OK : OUT_OK_AND_FALSE;
}

// Check if a string starts with a fragment
Outcome dynstring_do_substring_at(DynString* src, Int64 pos, DynString* frag) {
    if (src == NULL || frag == NULL) return OUT_NULL_INPUT;
    if (pos < 0 || pos >= src->len || pos + frag->len > src->len) return OUT_INVALID_INPUT;

    Bool res = strncmp(src->data + pos, frag->data, frag->len) == 0;

    return res ? OUT_OK : OUT_OK_AND_FALSE;
}

// Find a fragment in a string
Outcome dynstring_do_find(DynString* src, Int64* result_at, DynString* frag) {
    if (src == NULL || frag == NULL) return OUT_NULL_INPUT;
    if (frag->len > src->len) return OUT_INVALID_INPUT;

    for (Int64 i = 0; i < src->len; i++) {
        if (dynstring_do_substring_at(src, i, frag)) {
            *result_at = i;
            return OUT_OK;
        }
    }

    return OUT_OK_AND_FALSE;
}

// Print string
Outcome dynstring_do_print(DynString* str, bool print_newline) {
    if (str == NULL) return OUT_NULL_INPUT;

    printf("%s", str->data);
    if (print_newline) printf("\n");

    return OUT_OK;
}