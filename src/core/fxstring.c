#include <string.h>

#include "seawitch.h"
// Fixed string implmentation

FxString fxstring_do_from_chars(char* frag, Bool truncate_at_beginning) {
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
        if (truncate_at_beginning) {
            strncpy(str.data, frag + len - (FIXED_STRING_SIZE - 1), (FIXED_STRING_SIZE - 1));
            str.data[FIXED_STRING_SIZE - 1] = '\0';

            // Add ellipsis if the string was truncated
            str.data[0] = '.';
            str.data[1] = '.';
            str.data[2] = '.';        
        } else {
            strncpy(str.data, frag, FIXED_STRING_SIZE - 1);
            str.data[FIXED_STRING_SIZE - 1] = '\0';

            // Add ellipsis if the string was truncated
            str.data[FIXED_STRING_SIZE - 4] = '.';
            str.data[FIXED_STRING_SIZE - 3] = '.';
            str.data[FIXED_STRING_SIZE - 2] = '.';
        }
        str.len = FIXED_STRING_SIZE - 1;
    }

    return str;
}