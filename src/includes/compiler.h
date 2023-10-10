#include <stdio.h>

#ifndef HX_COMPILER_H
#define HX_COMPILER_H

typedef enum
{
    TOK_EOF,                    // End of file      0
    TOK_ILLEGAL,                // Illegal token    1
    TOK_SPACE,                  // Space            2
    TOK_VAR,                    // var              3
    TOK_COMMENT_START,                    // var              3
    TOK_COMMENT_END,                    // var              3
    TOK_CONST,                  // const            4
    TOK_ASSIGN,                 // =                5
    TOK_INT_LIT,                // Integer literal  6
    TOK_FLOAT_LIT,              // Float literal    7
    TOK_EXPO_LIT,              // Float literal    7
    TOK_BOOL_LIT,               // Bool literal     8
    TOK_IDENTIFIER,             // Identifier       9
} Hx_Token_Kind;

typedef struct Hx_Token
{
    char*           kind;
    char*           value;
    fpos_t          pos;
    int             line;
} Hx_Token;


typedef enum {
    // General Errors
    ERR_OPENING_FILE,
    ERR_READING_FILE,
    ERR_OUT_OF_MEMORY,
    ERR_MEMORY_ALLOCATION,

    // Compiler Errors
    UNEXPECTED_TOKEN,
    UNEXPECTED_EOF,
    UNCLOSED_STRING,
    UNCLOSED_COMMENT,
    UNCLOSED_PAREN,
    UNCLOSED_BRACE,
    UNCLOSED_BRACKET,
    UNCLOSED_ANGLE_BRACKET,

    // Compiler Warnings
    UNUSED_IDENTIFIER,
    DEPRECATED_FEATURE,

    // 

} Hx_Compiler_Error_Types;

char* read_file (char* );
List* lex_file(char* src);
int compile_file (char*);

// define an array of tokens 


#endif // HX_COMPILER_H