#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "seawitch.h"
#include "string.h"

DynString* read_file(char* filepath) {
    // Open the file in read-only mode
    FILE* file = fopen(filepath, "r");
    if (!file) fatal_read_file_failure(__LINE__, __FILE__);

    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a string to hold the file contents
    DynString* contents = calloc(1, sizeof(DynString));
    if (!contents) fatal_memory_failure(__LINE__, __FILE__);

    contents->len = file_size;
    contents->capacity = file_size + 1;
    contents->data = calloc(contents->capacity, sizeof(char));
    if (!contents->data) fatal_memory_failure(__LINE__, __FILE__);

    // Read the file contents into the string
    size_t bytes_read = fread(contents->data, 1, file_size, file);

    // Make sure the string is null-terminated
    contents->data[file_size] = '\0';

    // Clean up
    fclose(file);

    return contents;
}


// char* read_file(char *filepath)
// {

//     // Open the file in read-only mode
//     FILE *file = fopen(filepath, "r");
//     if (!file)
//     {
//         printf("Failed to open file '%s'\n", filepath);
//         // return 1; -- TODO panic? log levels?
//     }

//     // Determine the file size
//     fseek(file, 0, SEEK_END);
//     size_t file_size = ftell(file);
//     fseek(file, 0, SEEK_SET);

//     // Allocate a string to hold the file contents
//     char *contents = calloc(1, (file_size + 1));
//     if (!contents)
//     {
//         printf("Failed to allocate memory for file contents\n");
//         fclose(file);
//         // return 1;
//     }

//     // Read the file contents into the string
//     size_t bytes_read = fread(contents, 1, file_size, file);

//     // Make sure the string is null-terminated
//     contents[file_size] = '\0';

//     // Clean up
//     // free(contents);
//     fclose(file);

//     return contents;
// }
