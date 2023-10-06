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
    Hx_Token_Kind   kind;
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

// Define the array structure
typedef struct {
    void** data;     // Array of void pointers (references to items)
    size_t size;     // Current number of items in the array
    size_t capacity; // Maximum capacity of the array
} List;

// Function to create a new dynamic array with an initial capacity
List* list_new(size_t initial_capacity);
// Function to push an item (reference) into the dynamic array
void list_push(List* array, void* item);
// Function to pop an item (reference) from the dynamic array
void* list_pop(List* array);
// Function to get an item (reference) from the dynamic array by index
void* list_get(const List* array, size_t index);
// Function to destroy the dynamic array and free its memory
void list_destroy(List* array);


char* read_file (char* );
List* lex_file(char* src);
int compile_file (char*);

// define an array of tokens 


#endif // HX_COMPILER_H