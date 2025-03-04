#include <string.h>

#include "seawitch.h"
#include "errors.h"
// Fixed string implmentation

FxString fxstring_from_cstr(char* frag, bool keep_end) {
    FxString str;
    str[0] = '\0';

    if (frag == NULL) return str;

    size_t len = strlen(frag);
    if (len < FIXED_STRING_SIZE) {
       // If the string is short, copy it and pad with null characters
        strncpy(str, frag, len);
        str[len + 1] = '\0';

    } else {
        // If the string is too long, truncate while keeping only the last FIXED_STRING_SIZE - 1 characters    
        if (keep_end) {
            strncpy(str, frag + len - (FIXED_STRING_SIZE - 1), (FIXED_STRING_SIZE - 1));
            str[0] = '.';
            str[1] = '.';
            str[2] = '.';
        } else {
            strncpy(str, frag, FIXED_STRING_SIZE - 1);
            str[FIXED_STRING_SIZE - 4] = '.';
            str[FIXED_STRING_SIZE - 3] = '.';
            str[FIXED_STRING_SIZE - 2] = '.';
        }
        str[FIXED_STRING_SIZE - 1] = '\0';
    }
    return str;
}

DynString* dynstring_from_cstr(char* frag) {
    DynString* str = calloc(1, sizeof(DynString));
    if (!str) fatal_memory_failure(__LINE__, __FILE__);

    str->len = strlen(frag);
    str->capacity = str->len + 1;
    str->data = calloc(str->capacity, sizeof(char));
    if (!str->data) return NULL;

    char* res = strcpy(str->data, frag);
    if (!res) fatal_memory_failure(__LINE__, __FILE__);

    return str;
}