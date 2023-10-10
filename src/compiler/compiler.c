#include "common.h"
#include "helpers.h"

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
    // showing in a tabular formar, cuz we are cool like that
    printf("| %-5s | %-15s | %-20s | %-5s | %-5s |\n", "#", "Kind", "Value", "Pos", "Line");
    printf("|-------|-----------------|----------------------|-------|-------|\n");
    for (size_t i = 0; i < tokenList->size; i++) {
        Hx_Token* token = (Hx_Token*)list_get(tokenList, i);
        if (token != NULL) {
            // if (strcmp(token->kind, "SPACE") != 0) {
            printf("| %-5zu | %-15s | %-20s | %-5lld | %-5d |\n",
                 i+1, token->kind, token->value, token->pos, token->line);
            // }
        }
    }
    printf("|-------|-----------------|----------------------|-------|-------|\n");


    // Clean up and free memory
    list_destroy(tokenList);


    // free the memory
    free(content);
    return 0;
}