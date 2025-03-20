#ifndef SEAWITCH_COMPILER_H
#define SEAWITCH_COMPILER_H

#include <stdbool.h>
#include <stdio.h>

#include "sw_bootstrap.h"


// Define the macro for the enum and string array
#define TOKEN_KINDS     \
    X(TOKEN_NONE)       \
    X(TOKEN_ASSIGN)     \
    X(TOKEN_IDENTIFIER) \
    X(TOKEN_INTEGER)    \
    X(TOKEN_DECIMAL)    \
    X(TOKEN_CONST)      \
    X(TOKEN_VAR)        \
    X(TOKEN_COUNT_X)

// Generate the enum using the macro
typedef enum
{
#define X(token) token,
    TOKEN_KINDS
#undef X
} Token_kind_enum;

// Generate the string array using the macro
static const char *Token_kind_strings[] = {
#define X(token) #token,
    TOKEN_KINDS
#undef X
};


// Define the macro for the enum and string array
#define NODE_KINDS      \
    X(NODE_NONE)        \
    X(NODE_PROGRAM)     \
    X(NODE_STATEMENTS)  \
    X(NODE_DECLRATION)  \
    X(NODE_IDENTIFIER)  \
    X(NODE_INTEGER)     \
    X(NODE_DECIMAL)     \
    X(NODE_COUNT_X)

// Generate the enum using the macro
typedef enum
{
#define X(astnode) astnode,
    NODE_KINDS
#undef X
} Node_kind_enum;

// Generate the string array using the macro
static const char *Node_kind_strings[] = {
#define X(astnode) #astnode,
    NODE_KINDS
#undef X
};

typedef struct Node Node; // Forward declaration
struct Node
{
    Token_kind_enum token_kind;
    Node_kind_enum node_kind;

    size_t start;
    size_t end;
    size_t line;
    size_t column;

    // size_t depth;         // distance from root
    // Node *parent;      // immediate parent
    // Node *scope_owner; // nearest scope owner

    // Node specific data
    // union {
    //     struct {
    //         bool is_var;
    //         bool is_new;
    //         bool is_assignment;
    //         Node *identifier_node;
    //         Node *expression_node;
    //     } Declaration;
    //     struct {
    //         Node** statements_list;
    //         size_t statements_len;
    //         size_t statements_capacity;
    //     } Block;
    //     struct {
    //         Node *block_node;
    //     } Program;
    // } props;
};

// static const DynString* default_c_code = dynstring_do_create("\
// int main() {\n\
//     return 0;\n\
// }");

typedef struct {
    // input source
    char* filepath;
    char* src;
    size_t src_len;

    // output source
    char* code;
    size_t code_len;

    // cursors 
    size_t i;

    CError* errors_list;
    size_t errors_len;
    size_t errors_capacity;

    Node* nodes_list;
    size_t nodes_len;
    size_t nodes_capacity;

    // durations
    double reading_duration;
    double lexing_duration;
    double parsing_duration;
} Compiler_context;

void transpile_code(Compiler_context *ctx);
Compiler_context *transpiler_ctx_do_init(char *filepath, char *src);
// void transpiler_ctx_do_push_error(Transpiler_context *ctx, Compiler_error *err);

void lex_code(Compiler_context *ctx);


#endif