#include "common.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"


// Read file function. Take in a string filepath and returns a string of the file contents
char* read_file (char* filepath) {
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        perror("Failed to open file");
        perror("Memory allocation failed");
        exit(1);
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the string to hold the file contents
    char *content = (char *)malloc(file_size + 1); // +1 for null terminator

    if (content == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        perror("Memory allocation failed");
        fclose(file);
        exit(1);
    }

    // Read the file contents into the string
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file);

    // return the file content
    return content;
}

