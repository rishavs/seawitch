#ifndef SEAWITCH_SYSTEM_H
#define SEAWITCH_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "seawitch.h"

// Program exits with error message if memory allocation fails
void* strict_malloc(size_t size, char* in_file, size_t at_line);

// Program exits with error message if memory allocation fails
void* strict_calloc(size_t num, size_t size, char* in_file, size_t at_line);

// Program exits with error message if memory allocation fails
void* strict_realloc(void* ptr, size_t size, char* in_file, size_t at_line);

typedef struct {
    char*   message;                // Error message

    char*   name;                   // Error header. Optional
    char*   details;                // Optional. cause. hint. etc.

    int64_t pos;                    // Error position provided by user. Optional
    int64_t line;                   // Error line number provided by user. Optional
    int64_t column;                 // Error column number provided by user. Optional
    char*   filepath;               // Error file path provided by user. Optional
    
    int64_t raised_on_line;         // Internal compiler path for error.
    char*   raised_in_file;         // Internal compiler path for error.
} Compiler_error;
Compiler_error* snitch(char* msg, char* in_file, size_t at_line);
void yell(Compiler_error* err);
        
#endif // SEAWITCH_SYSTEM_H