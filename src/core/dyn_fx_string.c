#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "errors.h"
#include "seawitch.h"

// Create a fixed string. If the string is too long, truncate it.
FxString fxstring_create(char* frag) {
    // empty null terminated string
    FxString str = { .len = 0, .data = {'\0'} };
    if (frag == NULL) return str;

    size_t len = strlen(frag);
    if (len < FIXED_STRING_SIZE) {
       // If the string is short, copy it and pad with null characters
        strncpy(str.data, frag, len);
        str.len = len;
        str.data[len] = '\0';

    } else {
        // If the string is too long, truncate while keeping only the last FIXED_STRING_SIZE - 1 characters    
        strncpy(str.data, frag, FIXED_STRING_SIZE - 1);

        // Add ellipsis if the string was truncated
        str.data[FIXED_STRING_SIZE - 4] = '.';
        str.data[FIXED_STRING_SIZE - 3] = '.';
        str.data[FIXED_STRING_SIZE - 2] = '.';
        str.data[FIXED_STRING_SIZE - 1] = '\0';

        str.len = FIXED_STRING_SIZE - 1;
    }

    return str;
}

// Create a fixed string from the end of a string. 
// If the string is too long, truncate it from beginning
FxString fxstring_create_from_behind(char* frag) {
    // empty null terminated string
    FxString str = { .len = 0, .data = {'\0'} };
    if (frag == NULL) return str;

    size_t len = strlen(frag);
    if (len < FIXED_STRING_SIZE) {
       // If the string is short, copy it and pad with null characters
        strncpy(str.data, frag, len);
        str.len = len;
        str.data[len] = '\0';
    } else {
        // If the string is too long, truncate while keeping only the last FIXED_STRING_SIZE - 1 characters
        strncpy(str.data, frag + len - (FIXED_STRING_SIZE - 1), (FIXED_STRING_SIZE - 1));

        // Add ellipsis if the string was truncated
        str.data[0] = '.';
        str.data[1] = '.';
        str.data[2] = '.';        
        str.len = FIXED_STRING_SIZE - 1;
        str.data[FIXED_STRING_SIZE - 1] = '\0';
    }

    return str;
}

// Create a new dynamic string. 
DynString* dynstring_create() {
    
    DynString* str = calloc(1, sizeof(DynString));
    if (str == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    str->len = 0;
    str->capacity = FIXED_STRING_SIZE;
    str->data = calloc(str->capacity, sizeof(char));
    if (str->data == NULL) fatal(snitch("Memory error", __LINE__, __FILE__));

    str->data[0] = '\0'; 

    return str;
}

// append a fixed-string or char* to a string
Error dynstring_push_chars(DynString* src, char* data) {
     if (src == NULL || data == NULL) return snitch("Null input", __LINE__, __FILE__);

    size_t str_size = strlen(data);
    if (str_size > INT64_MAX) return snitch("Integer overflow", __LINE__, __FILE__);
    Int64 len = (Int64)str_size;

    char* temp = realloc(src->data, src->len + len + 1);
    if (temp == NULL) return snitch("Memory error", __LINE__, __FILE__);
    src->data = temp;

    memcpy(src->data + src->len, data, len);
    src->len += len;
    src->data[src->len] = '\0'; // Explicit null termination

    return (Error){ .ok = true };
}

// Get a substring from a string, given a start and end position
Error dynstring_slice (DynString* src, DynString* result, Int64 start, Int64 end) {
    if (src == NULL || result == NULL ) return snitch("Null input", __LINE__, __FILE__);

    if (    
        start < 0 || start >= src->len || 
        end < start || end >= src->len
    ) return snitch("Invalid input", __LINE__, __FILE__);

    result->len = end - start + 1;
    result->capacity = result->len + 1;
    result->data = calloc(result->capacity, sizeof(char));
    if (result->data == NULL) return snitch("Memory error", __LINE__, __FILE__);

    memcpy(result->data, src->data + start, result->len);

    result->data[result->len] = '\0';
    
    return (Error){ .ok = true };
}

Error dynstring_join(DynString* result, Int64 n, ...) {
    if (result == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (n <= 0) return snitch("Invalid input", __LINE__, __FILE__);

    va_list args;
    va_start(args, n);

    // Calculate total length
    Int64 total_len = 0;
    for (Int64 i = 0; i < n; i++) {
        DynString* str = va_arg(args, DynString*);
        if (str == NULL) {
            va_end(args);
            return snitch("Null input", __LINE__, __FILE__);   // That specific input string is not formed
        }
        total_len += str->len;
    }
    va_end(args);

    // allocate the result string
    result->len = total_len;
    result->capacity = total_len + 1;
    result->data = calloc(result->capacity, sizeof(char));
    if (result->data == NULL) return snitch("Memory error", __LINE__, __FILE__);

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

    return (Error){ .ok = true };
}

// Compare two strings
Error dynstring_compare(DynString* str1, DynString* str2, Bool* result) {
    if (str1 == NULL || str2 == NULL) return snitch("Null input", __LINE__, __FILE__);
    if (str1->len != str2->len) {
        *result = false;
        return (Error){ .ok = true };
    }

    *result = strncmp(str1->data, str2->data, str1->len) == 0;

    return (Error){ .ok = true };
}

// // Check if a string starts with a fragment
// Error dynstring_do_substring_at(DynString* src, Int64 pos, DynString* frag, Bool* result) {
//     if (src == NULL || frag == NULL) return snitch("Null input", __LINE__, __FILE__);
//     if (pos < 0 || pos >= src->len || pos + frag->len > src->len) return snitch("Invalid input", __LINE__, __FILE__);

//     *result = strncmp(src->data + pos, frag->data, frag->len) == 0;

//     return (Error){ .ok = true };
// }

// // Find a fragment in a string
// Error dynstring_do_find(DynString* src, DynString* frag, Int64* result_at) {
//     if (!src || !frag) return snitch("Null input", __LINE__, __FILE__);
//     if (frag->len > src->len) {
//         *result_at = -1; // Indicate not found
//         return (Error){.ok = true};
//     }

//     for (Int64 i = 0; i <= src->len - frag->len; i++) { 
//         Bool found = false;
//         Error err = dynstring_do_substring_at(src, i, frag, &found);
//         if (!err.ok) return err;

//         if (found) {
//             *result_at = i;
//             return (Error){.ok = true};
//         }
//     }

//     *result_at = -1; // Indicate not found
//     return (Error){.ok = true};
// }
// // Print string
// Error dynstring_do_print(DynString* str, Bool print_newline) {
//     if (str == NULL) return snitch("Null input", __LINE__, __FILE__);

//     printf("%s", str->data);
//     if (print_newline) printf("\n");

//     return (Error){ .ok = true };
// }