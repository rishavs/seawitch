#include <string.h>

#include "seawitch.h"
// Fixed string implmentation

FxString fxstring_do_create(char* data) {
    if (data == NULL) return "";

    FxString str;

    size_t str_size = strlen(data);
    if (str_size > INT_MAX) return OUT_INTEGER_OVERFLOW;
    Int64 len = (Int64)str_size;

    // If the string is too long, truncate from the beginning,
    // keeping only the last FIXED_STRING_SIZE characters
    if (len > FIXED_STRING_SIZE) {
        strncpy(str, data + len - FIXED_STRING_SIZE, FIXED_STRING_SIZE);
        str[FIXED_STRING_SIZE - 1] = '\0'; // Ensure null-termination
    } else {
        // If the string is short, copy it and pad with null characters
        strncpy(str, data, FIXED_STRING_SIZE);
        str[len] = '\0'; // Ensure null-termination
    }

    return str;
}

FxString fxstring_do_from_chars(char* data) {
    if (data == NULL) return "";

    FxString str;

    size_t str_size = strlen(data);
    if (str_size > INT_MAX) return OUT_INTEGER_OVERFLOW;
    Int64 len = (Int64)str_size;

    // If the string is too long, truncate from the beginning,
    // keeping only the last FIXED_STRING_SIZE characters
    if (len > FIXED_STRING_SIZE) {
        strncpy(str, data + len - FIXED_STRING_SIZE, FIXED_STRING_SIZE);
        str[FIXED_STRING_SIZE - 1] = '\0'; // Ensure null-termination
    } else {
        // If the string is short, copy it and pad with null characters
        strncpy(str, data, FIXED_STRING_SIZE);
        str[len] = '\0'; // Ensure null-termination
    }

    return str;
}