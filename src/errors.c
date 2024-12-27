#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "errors.h"

// Memory allocaltion failed. Fatal failure.
void memory_allocation_failure(int64_t pos, int64_t line, char* filepath, char* transpiler_file, int64_t transpiler_line) {
    fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Memory Failure! Failed to allocate memory during compilation.");
    if (pos && line && filepath) { // memory failures need not be tied to user code
        fprintf(stderr, "Problem found at %zu:%zu in the file \"%s\"\n", pos, line, filepath);
    };   
    perror("Internal Note");
    fprintf(stderr, "Internal Note: Failed in the compiler code at line %zu in the file \"%s\"", transpiler_line, transpiler_file);
    fprintf(stderr, "\n\033[0m");
    exit(EXIT_FAILURE);
}

// void fatal 