#include <stdio.h>
#include <stdlib.h>

#include "seawitch.h"
#include "sw_string.h"

void* strict_malloc(size_t size, char* in_file, size_t at_line) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Memory Failure! Failed to allocate memory");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", in_file, at_line);
        if (errno != 0) {
            perror("MALLOC FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* strict_calloc(size_t num, size_t size, char* in_file, size_t at_line) {
    void* ptr = calloc(num, size);
    if (!ptr) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Memory Failure! Failed to allocate memory");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", in_file, at_line);
        if (errno != 0) {
            perror("CALLOC FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* strict_realloc(void* ptr, size_t size, char* in_file, size_t at_line) {
    ptr = realloc(ptr, size);
    if (!ptr) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Memory Failure! Failed to allocate memory");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", in_file, at_line);
        if (errno != 0) {
            perror("REALLOC FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* strict_fopen(char* filepath, char* mode, char* in_file, size_t at_line) {
    FILE* file = fopen(filepath, mode);
    if (!file) {
        fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] File Open Failure! Failed to open file");
        fprintf(stderr, "Compiler error path:: %s:%zu\n", in_file, at_line);
        if (errno != 0) {
            perror("FOPEN FAILED");
        }
        fprintf(stderr, "\n\033[0m");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Print error message and continue
void fatal_read_file_failure(size_t at_line, char* in_file) {
    fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Read Failure! Unable to read the source file");
    fprintf(stderr, "Compiler error path:: %s:%zu\n", in_file, at_line);
    if (errno != 0) {
        perror("ErrNo Details");
    }
    fprintf(stderr, "\n\033[0m");
    exit(EXIT_FAILURE);
}

// Build a short error object with minimal ceremony. Heap allocated for passing around
Compiler_error* snitch(char* msg, char* in_file, size_t at_line) {
    Compiler_error* err = strict_calloc(1, sizeof(Compiler_error), in_file, at_line);

    err->message        = dynstring_from_cstr(msg);
    err->raised_on_line = at_line;
    err->raised_in_file = dynstring_from_cstr(in_file);

    return err;
}
    
// Print error message and continue
void yell(Compiler_error* err) {
    // printing in format <filename>:<line>:<column>: <message> makes it clickable in IDEs and editors
    fprintf(stderr, "\033[0;31m");
    if (err->name) {
        fprintf(stderr, "[ %s ]", err->name->data);
    } else {
        fprintf(stderr, "[ ERROR ]");
    }
    if (err->message) {
        fprintf(stderr, " %s", err->message->data);
    } else {
        fprintf(stderr, " Unknown error");
    }
    if (err->details) {
        fprintf(stderr, "\n%s", err->details->data);
    } else {
        fprintf(stderr, "\n%s", "Unknown error details");
    }
    if (err->filepath) {
        fprintf(stderr, "\n%s", err->filepath->data);
    } else {
        fprintf(stderr,"\n%s", "Unknown file");
    }
    if (err->line) {
        fprintf(stderr, ":%lli", err->line);
    } else {
        fprintf(stderr, "");
    }
    if (err->column) {
        fprintf(stderr, ":%lli", err->column);
    } else {
        fprintf(stderr, "");
    }
    
    fprintf(stderr, "\nCompiler error path:: %s:%zu\n", err->raised_in_file->data, err->raised_on_line);
    
    
    if (errno != 0) {
        perror("ERRNO Details");
    }

    fprintf(stderr, "\n\033[0m");
}