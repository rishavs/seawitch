#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
// #include "hexal.h"


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

    int line;

    Token* prev_token;
    Token* curr_token;
    Token* next_token;

} Hx_File_Parsing_Context;


typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} String;

String* init_string() {
    String* string = malloc(sizeof(String));
    if (string == NULL) {
        printf("Error: Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    string->length = 0;
    string->capacity = 8;
    string->data = malloc(string->capacity * sizeof(char));
    if (string->data == NULL) {
        printf("Error: Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    string->data[0] = '\0';

    return string;
}

// Function to append a char to a String
String* append_char_to_string(String* string, int ch) {
    if (string->length + 1 >= string->capacity) {
        // Double the capacity
        string->capacity *= 2;

        // Reallocate memory with double the capacity
        string->data = realloc(string->data, string->capacity * sizeof(char));
        if (string->data == NULL) {
            printf("Error: Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }
    }

    // Append the character to the string
    string->data[string->length] = (char) ch;
    string->data[string->length + 1] = '\0';
    string->length += 1;

    return string;
}

// Function to free the memory allocated for a String
void free_string(String* string) {
    free(string->data);
    free(string);
}

Token* get_next_token(Hx_File_Parsing_Context* ctx, Hx_Compiler_Metadata* meta) {
    // get the next token
    int ch = fgetc(ctx->fp);

    // setup return token
    Hx_Token_Type tok_type;
    String* tok_val = init_string();

    // Handle end of file
    if (ch == EOF) {
        tok_type = TOK_EOF;

        // Handle spaces & newlines. Roll all spaces into a single space token
    } else if (isspace(ch)) {
        do {
            if (ch == '\n') {
                ctx->line++;
            }
            ch = fgetc(ctx->fp);
        } while (isspace(ch));

        ungetc(ch, ctx->fp);
        tok_type = TOK_SPACE;

        // Handle Floats, exponentials and Integers
    } else if (isdigit(ch)) {
        tok_val = append_char_to_string(tok_val, ch);
        tok_type = TOK_INT_LIT;

        // roll all digits into a single float
        do {
            ch = fgetc(ctx->fp);
            if (ch == '_') {
                continue;
            } else if (ch == '.') {
                if (tok_type == TOK_INT_LIT) {
                    tok_val = append_char_to_string(tok_val, ch);
                    tok_type = TOK_FLOAT_LIT;
                } else {
                    ungetc(ch, ctx->fp);
                    break;
                }
            } else if (ch == 'e') {
                if (tok_type == TOK_INT_LIT || tok_type == TOK_FLOAT_LIT) {
                    tok_val = append_char_to_string(tok_val, ch);
                    tok_type = TOK_EXPO_LIT;
                } else {
                    ungetc(ch, ctx->fp);
                    break;
                }
            } else if (isdigit(ch)) {
                tok_val = append_char_to_string(tok_val, ch);
            } else {
                ungetc(ch, ctx->fp);
                break;
            }
        } while (isdigit(ch) || ch == '.' || ch == '_' || ch == 'e');

        // Handle Illegal Characters
    } else {
        tok_val = append_char_to_string(tok_val, ch);
        tok_type = TOK_ILLEGAL;
    }

    Token* res = malloc(sizeof(Token));
    res->type = tok_type;
    res->value = strdup(tok_val->data);
    res->line = ctx->line;
    free_string(tok_val);

    return res;
}

// parse_file
int parse_file(Hx_File_Parsing_Context* ctx, Hx_Compiler_Metadata* meta) {
    // advance the tokens to fill the prev, curr, and next tokens
    ctx->prev_token = ctx->curr_token;
    ctx->curr_token = ctx->next_token;
    ctx->next_token = get_next_token(ctx, meta);

    printf("\nAll Tokens in the file = \n");

    do {

        if (ctx->prev_token) {
            free(ctx->prev_token->value);
            free(ctx->prev_token);
        }

        // advance the tokens in a loop till file ends
        ctx->prev_token = ctx->curr_token;
        ctx->curr_token = ctx->next_token;
        ctx->next_token = get_next_token(ctx, meta);
        // printf("{ type: %i, value: %s, line: %i },\n", ctx->curr_token->type, ctx->curr_token->value, ctx->curr_token->line);
        printf("{ type: %i, value: %s, line: %i },\n", (int)ctx->curr_token->type, ctx->curr_token->value, ctx->curr_token->line);

    } while (ctx->curr_token->type != TOK_EOF);

    return 0;
}

// Section for handling multiple files
int compile_source(char* entry_file) {
    // parse = tokenize + parse + typecheck
    // compile = parse + generate
    // present = compile + present + run

    Hx_File_Parsing_Context ctx = {0};
    ctx.line = 1;

    Hx_Compiler_Metadata meta = {0};
    meta.Hx_Bytes_Compiled = 0;
    meta.Hx_Compiling_Duration = 0;
    meta.Hx_Compiler_Errors = NULL;

    ctx.fp = fopen(entry_file, "r, ccs=UTF-8");
    if (ctx.fp == NULL) {
        printf("Error: Failed to open file\n");
        exit(EXIT_FAILURE);
    }
    printf("Parsing file: %s\n", entry_file);
    int res = parse_file(&ctx, &meta);

    fclose(ctx.fp);

    return res;
}

int main(int argc, char* argv[]) {

    // if (argc == 1) {
    //     printf("Hexal Programming Language\n");
    //     printf("Version: %s\n", HX_VERSION);
    //     printf("Usage: hexal [options] <file>\n");
    //     printf("Options:\n");
    //     printf("  -h, --help\t\tPrint this help message\n");
    //     printf("  -v, --version\t\tPrint version information\n");
    //     return 0;
    // } else if (argc == 3 && strcmp(argv[1], "run") == 0) {
    //     printf("Compiling project with entrypoint file: %s\n", argv[2]);

    //     int res = compile_source(argv[2]);
    //     // parse = tokenize + parse + typecheck
    //     // compile = parse + generate
    //     // present = compile + present + run
    //     return res;
    // } else {
    //     printf("Invalid arguments\n");
    //     return 1;
    // }
    int res = compile_source("input/test.hex");

    return res;
}
