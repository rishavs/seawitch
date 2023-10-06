#include "common.h"

#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"


int compile_file (char* filepath) {
    // Read the file
    char* content = read_file(filepath);

    // char* content = "var x = 10;";

    // Check if the file is empty
    if (content == NULL) {
        perror("Empty File");
        return 1;
    }
    // print content
    printf("%s\n", content);

    // Build the list of tokens
    List* tokenList = lex_file(content);

    // Access and print the tokens in the list
    for (size_t i = 0; i < tokenList->size; i++) {
        Hx_Token* token = (Hx_Token*)list_get(tokenList, i);
        if (token != NULL) {
            printf("| kind=%d, value=%s, cursor=%lld, line=%d\n",
                 token->kind, token->value, token->pos, token->line);
        }
    }

    // Clean up and free memory
    list_destroy(tokenList);


    // free the memory
    free(content);
    return 0;
}