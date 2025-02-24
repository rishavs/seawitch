#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../include/seawitch.h"
#include "../include/compiler.h"


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