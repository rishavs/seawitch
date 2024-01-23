#include "common.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compiler.h"

void print_ast(const Hx_AST_Node* node) {
    if (node == NULL) {
        return;
    }

    // Print node information (kind, value, position, etc.)
    printf("Kind: %s, Value: %s, Depth: %d, Pos: %zu, Line: %d, Resolved: %s\n",
           node->kind, node->value, node->depth, node->pos, node->line,
           node->resolved ? "Yes" : "No");

    // Recursively print children
    if (node->children != NULL) {
        for (size_t i = 0; i < list_size(node->children); i++) {
            Hx_AST_Node* child = list_get(node->children, i);
            print_ast(child);
        }
    }
}

// Parser takes in a list of tokens and returns an AST
Hx_AST_Node* parse_file(List* tokens) {
    // Create a new AST
    Hx_AST_Node* ast = malloc(sizeof(Hx_AST_Node));
    if (ast == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    ast->kind = "PROGRAM";
    ast->value = NULL;
    ast->pos = 0;
    ast->line = 0;
    ast->children = list_new(1);

    return ast;
}