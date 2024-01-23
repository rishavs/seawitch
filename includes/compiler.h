#include <stdio.h>
#include "stc/cstr.h"


#ifndef SEAWITCH_COMPILER_H
#define SEAWITCH_COMPILER_H

typedef struct Token
{
    cstr        kind;
    cstr        value; // this needs to be dynalically allocated
    fpos_t      pos;
    size_t      line;
} Token;
#define i_TYPE ListOfTokens, Token // Container type (name, element type)
#include "stc/vec.h"

typedef struct ParsingContext {
    FILE *file;   
    Token* prev2ndToken;
    Token* prev1stToken;
    Token* currToken;
    Token* next1stToken;
    Token* next2ndToken;
} ParsingContext;

int compile_file (cstr);
cstr read_file (cstr);
ListOfTokens lex_file (cstr);

#endif // SEAWITCH_COMPILER_H