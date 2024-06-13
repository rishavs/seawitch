#include <stdbool.h>

#include "ast.h"

#ifndef SEAWITCH_COMPILER_H
#define SEAWITCH_COMPILER_H

// typedef enum Token_Type
// {
//     TOK_ILLEGAL,
//     TOK_EOF,
//     TOK_NEWLINE,
//     TOK_SPACE,
//     TOK_VAR,
//     TOK_CONST,
//     TOK_ASSIGN,
//     TOK_INT_LIT,
//     TOK_FLOAT_LIT,
//     TOK_BOOL_LIT,
//     TOK_ID,
// } Token_Type;

typedef struct Compiler_Error {
    char* type;
    char* message;
    int atPos;
    int atLine;
} Compiler_Error;

typedef struct Compiler_Errors_List
{
    Compiler_Error* errors;
    size_t count;
    size_t capacity;
} Compiler_Errors_List;

void push_error(Compiler_Errors_List* , Compiler_Error );

int compile_file(char* filepath);
char* read_file(char*);

typedef struct Token
{
    // Token_Type type;
    char* type;
    char* value;
    int atPos;
    int atLine;
} Token;

typedef struct Tokens_List
{
    Token* tokens;
    size_t count;
    size_t capacity;
} Tokens_List;

Compiler_Errors_List lex_file(Tokens_List*, char*);

typedef struct Leaf {
    char* type;
    bool expr;
    char* value;

    size_t pos;
    size_t line;
    char* filename;
    size_t depth;

    struct Leaf* parent;
    struct Leaf** children;
    size_t children_count;
    size_t children_capacity;
} Leaf;

Compiler_Errors_List parse_file(Leaf*, Tokens_List*);

void print_ast(Leaf*);

#endif