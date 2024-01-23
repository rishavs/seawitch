#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

#include <stc/cstr.h>

cstr read_file (cstr filepath) {
    FILE *file = fopen(cstr_str(&filepath), "r");

    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer to hold the file contents
    // We have to do this a fread cannot handle cstr
    char *buffer = (char *)malloc(file_size + 1); // +1 for null terminator

    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        exit(1);
    }

    // Read the file contents into the buffer
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Close the file
    fclose(file);

    // Create a cstr from the buffer
    cstr content = cstr_from(buffer);

    // Free the buffer
    free(buffer);

    // return the file content
    return content;
}