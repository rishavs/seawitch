#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "sw_bootstrap.h"
#include "seawitch.h"

void* strict_calloc(size_t num, size_t size, char* raised_in_file, size_t raised_on_line) {
    void* ptr = calloc(num, size);
    if (!ptr) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL MEMORY ERROR ] Failed to allocate memory");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", raised_in_file, raised_on_line);
        if (errno != 0) {
            perror("CALLOC FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* strict_realloc(void* ptr, size_t size, char* raised_in_file, size_t raised_on_line) {
    ptr = realloc(ptr, size);
    if (!ptr) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL MEMORY ERROR ] Failed to allocate memory");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", raised_in_file, raised_on_line);
        if (errno != 0) {
            perror("REALLOC FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Build a short error object with minimal ceremony. Heap allocated for passing around
CError* snitch(char* name, char* msg, char* raised_in_file, size_t raised_on_line) {
    CError* err = strict_calloc(1, sizeof(CError), raised_in_file, raised_on_line);

    err->name = strict_calloc(strlen(name) + 1, sizeof(char), raised_in_file, raised_on_line);
    strcpy(err->name, name);

    err->message = strict_calloc(strlen(msg) + 1, sizeof(char), raised_in_file, raised_on_line);
    strcpy(err->message, msg);

    err->raised_in_file = strict_calloc(strlen(raised_in_file) + 1, sizeof(char), raised_in_file, raised_on_line);
    strcpy(err->raised_in_file, raised_in_file);

    err->raised_on_line = raised_on_line;

    return err;
}
    
// Print error message and continue
void yell(CError* err) {
    // printing in format <filename>:<line>:<column>: <message> makes it clickable in IDEs and editors
    fprintf(stderr, "\033[0;31m[ %s ] %s", 
        
        err->name != NULL 
        ? err->name 
        : "ERROR",

        strlen(err->message) > 0 
        ? err->message 
        : "Unknown error"
    );

    if (err->details != NULL && strlen(err->details) > 0)
        fprintf(stderr, "\nError details - \t%s", err->details);

    fprintf(stderr, "\n-- Error in code at %s:%zu:%zu", 
        err->filepath != NULL
        ? err->filepath 
        : "Unknown file",
        err->line, err->column
    );

    if (err->raised_in_file && strlen(err->raised_in_file) > 0)
        fprintf(stderr, "\n-- Compiler error location %s:%zu\n", err->raised_in_file, err->raised_on_line);

    if (errno != 0) {
        perror("ERRNO Details");
    }

    fprintf(stderr, "\n\033[0m");
}

char* format_duration(clock_t ticks) {
    double duration_seconds = (double)ticks / CLOCKS_PER_SEC;

    // Extract the integer part for seconds
    long long seconds = (long long)duration_seconds;

    // Calculate the remaining fractional part for microseconds
    double fractional_part = duration_seconds - seconds;
    long long microseconds = (long long)(fractional_part * 1000000.0 + 0.5);

    // Ensure microseconds are within the valid range (0 to 999999)
    if (microseconds >= 1000000) {
        seconds++;
        microseconds -= 1000000;
    }

    // Calculate the required buffer size
    int buffer_size = snprintf(NULL, 0, "%lld.%06llds", seconds, microseconds) + 1;

    // Allocate memory for the string
    char* buffer = (char*)malloc(buffer_size * sizeof(char));

    if (buffer == NULL) {
        perror("Failed to allocate memory for duration string");
        return NULL;
    }

    // Format the string into the buffer
    sprintf(buffer, "%lld.%06llds", seconds, microseconds);

    return buffer;
}