#include <stdio.h>

#define HX_VERSION "0.0.1"

typedef enum
{
    TOK_EOF,                    // End of file      0
    TOK_ILLEGAL,                // Illegal token    1
    TOK_SPACE,                  // Space            2
    TOK_VAR,                    // var              3
    TOK_CONST,                  // const            4
    TOK_ASSIGN,                 // =                5
    TOK_INT_LIT,                // Integer literal  6
    TOK_FLOAT_LIT,              // Float literal    7
    TOK_EXPO_LIT,              // Float literal    7
    TOK_BOOL_LIT,               // Bool literal     8
    TOK_IDENTIFIER,             // Identifier       9
} Hx_Token_Type;

typedef struct Token
{
    Hx_Token_Type type;

    char *value;
    fpos_t cursor;
    int line;
} Token;

typedef enum {
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

typedef struct {
    Hx_Compiler_Error_Types type;
    const char* err_message;
    size_t at_pos;
    size_t at_line;
} Hx_Compiler_Error;

// a type to hold the list of Hx_Compiler_Error

// Compiler metadata
typedef struct {
    Hx_Compiler_Error* Hx_Compiler_Errors;
    
    size_t Hx_Compiling_Duration;
    size_t Hx_Bytes_Compiled;

} Hx_Compiler_Metadata;


typedef struct {
    FILE* fp;
    fpos_t cursor;

    char ch;
    size_t pos;
    size_t line;
    
    Token* prev_token;
    Token* curr_token;
    Token* next_token;

} Hx_File_Parsing_Context;















typedef enum {
    KW_VAR,
    // Other keywords...
    KW_COUNT // Keep track of the number of keywords
} Hx_supported_keywords;

typedef struct {
    const char* en_us;
    const char* hi_in;
} Hx_Kwd_localization;

typedef struct {
    Hx_Kwd_localization translations;
} Hx_Kwds;

// KeywordData keywordData[KW_COUNT] = {
//     [KW_VAR] = { .translations = { .en_us = "var", .hi_in = "परिवर्तनशील" } },
//     // Initialize other keywords...
// };
// Keyword keyword = KW_VAR;
// const char* en_us_translation = keywordData[keyword].translations.en_us;
// const char* hi_in_translation = keywordData[keyword].translations.hi_in;