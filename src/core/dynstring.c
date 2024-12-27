
#include <string.h>

#include "errors.h"
#include "seawitch.h"

// Create a new dynamic string
String* string_create(const char* data) {
    if (data == NULL) return NULL;
    Int64 len = strlen(data);

    String* str = calloc(1, sizeof(String));
    if (str == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    str->len = len;
    str->capacity = str->len + 1;
    str->data = calloc(str->capacity, sizeof(char));
    if (str->data == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    memcpy(str->data, data, len);
    if (str->data == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    str->data[len] = '\0'; 

    return str;
}

// Get a substring from a string, given a position and length
String* string_get_substring(String* src, Int64 pos, Int64 len) {
    if (src == NULL) return NULL;
    if (pos < 0 || pos >= src->len) return NULL;
    if (len < 0) return NULL;

    if (pos + len > src->len) len = src->len - pos;

    String* result = calloc(1, sizeof(String));
    if (result == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    result->len = len;
    result->capacity = result->len + 1;
    result->data = calloc(result->capacity, sizeof(char));
    if (result->data == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    memcpy(result->data, src->data + pos, len);
    if (result->data == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    result->data[len] = '\0';
    
    return result;
}

// append a c-string/char* to a string
String* string_push_cstr(String* src, const char* data) {
    if (!src || !data) return NULL;
    Int64 len = (Int64)(strlen(data));

    // Integer overflow check (important!)
    if (src->len + len + 1 < src->len || src->len + len + 1 < len) {
        return NULL; // Integer overflow
    }

    char* temp = realloc(src->data, src->len + len + 1);
    if (!temp) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);
    src->data = temp;

    memcpy(src->data + src->len, data, len);
    src->len += len;
    src->data[src->len] = '\0'; // Explicit null termination

    return src;
}

String* string_join(Int64 n, ...) {
    if (n <= 0) return NULL;

    va_list args;
    va_start(args, n);

    Int64 total_len = 0;
    for (Int64 i = 0; i < n; i++) {
        String* str = va_arg(args, String*);
        if (!str) {
            va_end(args);
            return NULL;
        }
        total_len += str->len;
    }
    va_end(args);

    String* result = calloc(1, sizeof(String));
    if (!result) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);
    result->len = total_len;
    result->capacity = total_len + 1;

    result->data = calloc(result->capacity, sizeof(char));
    if (!result->data) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);

    va_start(args, n);
    Int64 offset = 0;
    for (Int64 i = 0; i < n; i++) {
        String* str = va_arg(args, String*);
        memcpy(result->data + offset, str->data, str->len);
        offset += str->len;
    }
    va_end(args);
    result->data[result->len] = '\0'; // Explicit null termination

    return result;
}

// Compare two strings
Bool string_compare(String* str1, String* str2) {
    if (str1 == NULL || str2 == NULL) return false;
    if (str1->len != str2->len) return false;

    return strncmp(str1->data, str2->data, str1->len) == 0;
}

// Check if a string starts with a fragment
Bool string_starts_with(String* src, Int64 pos, String* frag) {
    if (src == NULL || frag == NULL) return false;
    if (pos < 0 || pos >= src->len) return false;
    if (pos + frag->len > src->len) return false;

    return strncmp(src->data + pos, frag->data, frag->len) == 0;
}
