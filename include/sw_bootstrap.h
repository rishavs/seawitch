#ifndef SEAWITCH_SYSTEM_H
#define SEAWITCH_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "seawitch.h"

// Program exits with error message if memory allocation fails
void* strict_calloc(size_t num, size_t size, char* raised_in_file, size_t raised_on_line);

// Program exits with error message if memory allocation fails
void* strict_realloc(void* ptr, size_t size, char* raised_in_file, size_t raised_on_line);

typedef struct {
    char* message;                 // Error message

    char* name;                    // Error header. Optional
    char* details;                 // Optional. cause. hint. etc.

    size_t pos;                     // Error position provided by user. Optional
    size_t line;                    // Error line number provided by user. Optional
    size_t column;                  // Error column number provided by user. Optional
    char* filepath;                // Error file path provided by user. Optional
    
    size_t raised_on_line;          // Internal compiler path for error.
    char*  raised_in_file;   // Internal compiler path for error.
} CError;
CError* snitch(char* name, char* msg, char* raised_in_file, size_t raised_on_line);
void yell(CError* err);

// formatting time duration in HH:MM:SS:MS format
char* format_duration(clock_t ticks);
        
#endif // SEAWITCH_SYSTEM_H