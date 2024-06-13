#include <stdlib.h>

#include "../include/compiler.h"
#include "../include/ast.h"

void push_leaf(Leaf* parent, Leaf* child) {
    if (parent->children_count >= parent->children_capacity) {
        parent->children_capacity *= 2;
        parent->children = realloc(parent->children, parent->children_capacity * sizeof(Leaf*));
        if (parent->children == NULL) { perror("realloc failed"); };
    }
    parent->children[parent->children_count] = child;
    parent->children_count++;
}

Compiler_Errors_List parse_file(Leaf* root , Tokens_List* tokens_list) {
    root->type = "ROOT";
    root->expr = false;
    root->value = NULL;
    root->pos = 0;
    root->line = 1;
    root->depth = 0;
    root->filename = "main";
    root->parent = NULL;
    root->children_count = 0;
    root->children_capacity = 8;
    root->children = calloc(root->children_capacity, sizeof(Leaf*));
    if (root->children == NULL) { perror("calloc failed"); };

    Compiler_Errors_List errors_list;
    errors_list.count = 0;
    errors_list.capacity = 8;
    errors_list.errors = calloc(errors_list.capacity, sizeof(Compiler_Error));
    if (errors_list.errors == NULL) { perror("calloc failed"); };

    // Rest of your code...


    // printf("Printing Tokens inside the parser\n");
    // printf("num tokens = %d\n", tokens.count);
    // for (size_t i = 0; i < tokens.count; i++) {
        
    //     if (strcmp(tokens.tokens[i].type, "EOF") == 0) {
    //         break;
    //     } else if (strcmp(tokens.tokens[i].type, "ILLEGAL") == 0)  {
    //         puts("Found illegal");
    //         break;
    //     } else if (strcmp(tokens.tokens[i].type, "VAR") == 0)  {

    //         puts("Found Var");
    //     } else {
    //         printf("    { type: %s, value: %s, atPos: %d, atLine: %d },\n", 
    //         tokens.tokens[i].type, 
    //         tokens.tokens[i].value, 
    //         tokens.tokens[i].atPos, 
    //         tokens.tokens[i].atLine
    //     );
    //     } 
    // }

    return errors_list;
}

void print_ast(Leaf* root) {
    if (root == NULL) {
        printf("Error: NULL pointer passed to print_ast\n");
        return;
    }

    for (size_t i = 0; i < root->depth; i++) {
        printf("  ");
    }

    printf("Node:\n");
    printf("  type: %s\n", root->type);
    printf("  expr: %s\n", root->expr ? "true" : "false");
    printf("  value: %s\n", root->value);
    printf("  pos: %zu\n", root->pos);
    printf("  line: %zu\n", root->line);
    printf("  filename: %s\n", root->filename);
    printf("  depth: %zu\n", root->depth);
    printf("  children_count: %zu\n", root->children_count);

    for (size_t i = 0; i < root->children_count; i++) {
        print_ast(root->children[i]);
    }
}