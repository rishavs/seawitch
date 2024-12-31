#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

#include "seawitch.h"

Bool int64_do_to_bool(Int64 value) {
    return value != 0;
}

Float int64_do_to_float64(Int64 value) {
    return (float)value;
}

// Convert an Int64 value to a Byte value. Returns 0 on failure.
Byte int64_do_to_byte(Int64 value) {
    if (value < 0 || value > 255) {
        return 0;
    }
    
    return (Byte)value;
}

// Convert an Int64 value to a string. Returns NULL on failure.
String* int64_do_to_string(Int64 value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lld", value);
    return dynstring_create(buffer);
}

// Convert a string to an Int64 value. Returns -1 on failure.
Int64 int64_do_from_string(String* str) {
    if (str == NULL) return 0;

    Int64 res = atoll(str->data);
    return res == 0 ? -1 : res;
}

// Compare two Int64 values
Bool int64_do_equals(Int64 a, Int64 b) {
    return a == b;
}

// Copy an Int64 value
Int64 int64_do_copy(Int64 value) {
    return value;
}

// Internal Only. Convert a size_t to an Int64 value.
Int64 int64_do_from_string(size_t val) {
    if (val > INT_MAX) return -1;
    return (Int64)val;
}