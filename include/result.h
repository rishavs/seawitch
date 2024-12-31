#ifndef SEAWITCH_RESULT_H
#define SEAWITCH_RESULT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

typedef enum {
    // Special types
    SUCCESS,    // General success case
    EQUALITY,   // Success with equality result
    UNEQUALITY, // Success with inequality result

    // System errors
    MEMORY_ERROR,
    INTEGER_OVERFLOW,

    // User errors
    NULL_INPUT,
    INVALID_INPUT,

    // ...

    // Bookkeeping
    RESULT_CODES_COUNT
} Result_codes;

// Generate the string array using the macro
static const char* Result_message[RESULT_CODES_COUNT] = {
    [SUCCESS]                   = "[SUCCESS] Success",
    [EQUALITY]                  = "[SUCCESS] Equals",
    [UNEQUALITY]                = "[SUCCESS] Not Equals",

    [MEMORY_ERROR]              = "[FATAL] Memory Allocation Failure",
    [INTEGER_OVERFLOW]          = "[FATAL]Integer Overflow",
    [NULL_INPUT]                = "[FATAL] Null Input",

    [INVALID_INPUT]             = "[ERROR] Invalid Input",

};


#endif // SEAWITCH_RESULT_H