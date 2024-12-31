#ifndef SEAWITCH_ERRORS_H
#define SEAWITCH_ERRORS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

// Defined a light Outcome type for simple error handling
// May discard in favor of a proper Result type later on
typedef enum {
    // Special types
    OUT_OK,    // General success case. OK AND TRUE
    OUT_OK_AND_FALSE, // Success with inequality result

    // System errors
    OUT_MEMORY_ERROR,
    OUT_INTEGER_OVERFLOW,

    // User errors
    OUT_NULL_INPUT,
    OUT_INVALID_INPUT,

    // ...

    // Bookkeeping
    OUT_COUNT
} Outcome;

// Generate the string array using the macro
static const char* Outcome_message[OUT_COUNT] = {
    [OUT_OK]                = "[SUCCESS] No Errors",
    [OUT_OK_AND_FALSE]      = "[SUCCESS] No Errors, but false outcome",

    [OUT_MEMORY_ERROR]      = "[FATAL] Memory Allocation Failure",
    [OUT_INTEGER_OVERFLOW]  = "[FATAL] Integer Overflow",
    [OUT_NULL_INPUT]        = "[FATAL] Null Input",

    [OUT_INVALID_INPUT]     = "[ERROR] Invalid Input",

};


// FATAL! Print error message and exit
void fatal(Outcome code, Int64 line, FxString filepath);

// Print error message and continue
void yell_at_clouds(Outcome code, Int64 line, FxString filepath);

// // Print error message and continue
// void yell(Error err, FxString filepath, Int64 line);

// // Some pre built error messages
// // throw memory allocation failure and exit
// void fatal_memory_allocation (FxString filepath, Int64 line);

// // throw null input error and exit
// void fatal_null_input (FxString filepath, Int64 line);

// // throw integer overflow error and exit
// void fatal_integer_overflow (FxString filepath, Int64 line);

// // Prints an error message and returns NULL. Used in primitive functions with simple error handling
// void* print_error_return_null(char* transpiler_file, size_t transpiler_line);

// // Prints an error message and returns false. Used in primitive functions with simple error handling
// Bool print_error_return_false(char* transpiler_file, size_t transpiler_line);

// // Memory allocation failed. Fatal failure. Used in Transpiler functions
// void memory_allocation_failure(Int64 pos, Int64 line, char* filepath, char* transpiler_file, Int64 transpiler_line);

#endif // SEAWITCH_ERRORS_H