#include <stdio.h>
#include <string.h>
#include <time.h>

#include "seawitch.h"
#include "sw_string.h"
#include "sw_system.h"

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("Seawitch Programming Language\n");
        printf("Version: %s\n", SEAWITCH_VERSION);
        printf("Usage: hexal [options] <file>\n");
        printf("Options:\n");
        printf("  -h, --help\t\tPrint this help message\n");
        printf("  -v, --version\t\tPrint version information\n");
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "run") == 0) {
        printf("Compiling project with entrypoint file: %s\n", argv[2]);
        char* filepath = argv[2];
        // filepath = "notes.txt";

        // Open the file in read-only mode
        FILE* file = fopen(filepath, "r");
        if (!file) fatal_read_file_failure(__LINE__, __FILE__);

        // Determine the file size
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate a string to hold the file contents
        DynString* src = strict_calloc(1, sizeof(DynString));
        src->len = file_size;
        src->capacity = file_size + 1;       
        src->data = strict_calloc(src->capacity, sizeof(char));

        // Read the file src into the string
        size_t bytes_read = fread(src->data, 1, file_size, file);
        if (bytes_read < file_size) {
            if (ferror(file)) {
                perror("Error reading file");
                fclose(file);
                return 1;
            } else if (feof(file)) {
                printf("End of file reached\n");
            }
        }
    
        src->data[file_size] = '\0';

        // Clean up
        fclose(file);

        printf("1. Reading File: %s\n----\n%s\n----\n", filepath, src->data);

        // int res = compile_file(filepath);
        
        return 0;
        
    } else {
        printf("Invalid arguments\n");
        return 1;
    }

    return 0;
}


// #include "gc.h"
// #include <assert.h>
// #include <stdio.h>

// int main(void) {
//     printf("Starting\n");
//     int i;

//     GC_INIT();
//     GC_enable_incremental();  // Enables incremental GC
//     for (i = 0; i < 10000000; ++i) {
//         int **p = (int **) GC_MALLOC(sizeof(int *));
//         int *q = (int *) GC_MALLOC_ATOMIC(sizeof(int));
//         assert(*p == 0);
//         *p = (int *) GC_REALLOC(q, 2 * sizeof(int));
//         if (i % 100000 == 0)
//             printf("Heap size = %lu bytes\n",
//                    (unsigned long)GC_get_heap_size());
//     }
//     return 0;
// }