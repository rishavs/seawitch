#include <string.h>

#include "seawitch.h"
// Fixed string implmentation

FxString fxstring_do_from_chars(char* frag) {
    // empty null terminated string
    FxString str = { .len = 0, .data = {'\0'} };
    if (frag == NULL) return str;

    size_t len = strlen(frag);

    // If the string is too long, truncate from the beginning,
    // keeping only the last FIXED_STRING_SIZE - 1 characters
    if (len <= FIXED_STRING_SIZE) {
       // If the string is short, copy it and pad with null characters
        strncpy(str.data, frag, FIXED_STRING_SIZE);
        str.len = len;
        str.data[len] = '\0'; // Ensure null-termination
    } else {
        // If the string is too long, truncate from the beginning,
        strncpy(str.data, frag + len - FIXED_STRING_SIZE - 1, FIXED_STRING_SIZE - 1);
        str.len = FIXED_STRING_SIZE - 1;
        str.data[FIXED_STRING_SIZE - 1] = '\0'; // Ensure null-termination
    }

    return str;
}