#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seawitch.h"
#include "compiler.h"


int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("Hexal Programming Language\n");
        printf("Version: %s\n", HX_VERSION);
        printf("Usage: hexal [options] <file>\n");
        printf("Options:\n");
        printf("  -h, --help\t\tPrint this help message\n");
        printf("  -v, --version\t\tPrint version information\n");
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "build") == 0) {
        printf("Compiling project with entrypoint file: %s\n", argv[2]);
        int res = compile_file(argv[2]);
        return res;

        // int res = compile_source(argv[2]);
        // parse = tokenize + parse + typecheck
        // compile = parse + generate
        // present = compile + present + run
        // return 1;
    } else {
        printf("Invalid arguments\n");
        return 1;
    }

    return 0;
}
