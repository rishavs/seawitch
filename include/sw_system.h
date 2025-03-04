#ifndef SEAWITCH_SYSTEM_H
#define SEAWITCH_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#include "seawitch.h"
#include "sw_string.h"

// Program exits with error message if memory allocation fails
void* strict_malloc(size_t size, char* in_file, size_t at_line);

// Program exits with error message if memory allocation fails
void* strict_calloc(size_t num, size_t size, char* in_file, size_t at_line);

// Program exits with error message if memory allocation fails
void* strict_realloc(void* ptr, size_t size, char* in_file, size_t at_line);

// Build a short error object on the heap with minimal ceremony
Compiler_error* snitch(char* msg, char* in_file, size_t at_line);

// Print error message and continue
void yell(Compiler_error* err);

#endif // SEAWITCH_SYSTEM_H