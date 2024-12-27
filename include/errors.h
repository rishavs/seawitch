#ifndef SEAWITCH_ERRORS_H
#define SEAWITCH_ERRORS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"


// Prints an error message and returns NULL. Used in primitive functions with simple error handling
void* print_error_return_null(char* transpiler_file, size_t transpiler_line);

// Prints an error message and returns false. Used in primitive functions with simple error handling
Bool print_error_return_false(char* transpiler_file, size_t transpiler_line);

// Memory allocation failed. Fatal failure. Used in Transpiler functions
void memory_allocation_failure(Int64 pos, Int64 line, char* filepath, char* transpiler_file, Int64 transpiler_line);

#endif // SEAWITCH_ERRORS_H