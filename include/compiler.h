#ifndef SEAWITCH_COMPILER_H
#define SEAWITCH_COMPILER_H

#include <stdbool.h>
#include <stdio.h>

#include "sw_system.h"
#include "sw_string.h"

#include "seawitch.h"

typedef enum {
    TOKEN_VAR,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULUS,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
} Token_kind;

typedef struct {
    Token_kind kind;

    size_t start;
    size_t end;

    size_t line;
    size_t column;
} Token;

typedef struct {
    Compiler_error** error_refs_list;
    size_t error_len;
    size_t error_capacity;

    DynString* filepath;
    DynString* src;

    Token* tokens_list;
    size_t token_len;
    size_t token_capacity;
    
    double reading_duration;
    double lexing_duration;
} Transpiler_context;
void transpile_code (Transpiler_context* ctx) ;
Transpiler_context* transpiler_ctx_do_init (DynString* filepath, DynString* src);
void transpiler_ctx_do_push_error (Transpiler_context* ctx, Compiler_error* err);
void transpiler_ctx_do_push_token (Transpiler_context* ctx, Token token);

void lex_file(Transpiler_context* ctx);
// char* read_file(char*);

// typedef struct Token
// {
//     // Token_Type type;
//     char* type;
//     char* value;
//     int atPos;
//     int atLine;
// } Token;

// typedef struct Tokens_List
// {
//     Token* tokens;
//     size_t count;
//     size_t capacity;
// } Tokens_List;

// Compiler_Errors_List lex_file(Tokens_List*, char*);

// typedef struct Leaf {
//     char* type;
//     bool expr;
//     char* value;

//     size_t pos;
//     size_t line;
//     char* filename;
//     size_t depth;

//     struct Leaf* parent;
//     struct Leaf** children;
//     size_t children_count;
//     size_t children_capacity;
// } Leaf;

// Compiler_Errors_List parse_file(Leaf*, Tokens_List*);

// void print_ast(Leaf*);

#endif