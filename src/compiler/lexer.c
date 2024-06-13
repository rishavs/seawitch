#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/seawitch.h"
#include "../include/compiler.h"

bool starts_with(char *src, size_t src_len, size_t i,  char* frag) {
    size_t frag_len = strlen(frag);
    if (i + frag_len > src_len) { return false; }
    for (size_t j = 0; j < frag_len; j++) {
        if (src[i + j] != frag[j]) { return false; }
    }
    return true;
}

void push_token(Tokens_List* lexer, Token token) {
    if (lexer->count >= lexer->capacity) {
        lexer->capacity *= 2;
        lexer->tokens = realloc(lexer->tokens, lexer->capacity * sizeof(Token));
        if (lexer->tokens == NULL) { perror("realloc failed"); };
    }
    lexer->tokens[lexer->count] = token;
    lexer->count++;
}
// void push_token(Lexing_Result* lexer, Token token) {
//     if (lexer)
//     if (lexer->tok_count >= lexer->tok_capacity) {
//         lexer->tok_capacity *= 2;
//         lexer->tokens = realloc(lexer->tokens, lexer->tok_capacity * sizeof(Token));
//         if (lexer->tokens == NULL) { perror("realloc failed"); };
//     }
//     lexer->tokens[lexer->tok_count] = token;
//     lexer->tok_count++;
// }


Compiler_Errors_List lex_file(Tokens_List* result, char* src) {

    int src_len = strlen(src);
    int i = 0;
    int line = 1;

    result->count = 0;
    result->capacity = 8;
    result->tokens = calloc(result->capacity, sizeof(Token));
    if (result->tokens == NULL) { perror("calloc failed"); };

    Compiler_Errors_List errors_list;
    errors_list.count = 0;
    errors_list.capacity = 8;
    errors_list.errors = calloc(errors_list.capacity, sizeof(Compiler_Error));
    if (errors_list.errors == NULL) { perror("calloc failed"); };
    
    while (i < src_len && src[i] != '\0') {
        // Handle whitespace
        if (src[i] == ' ' || src[i] == '\t' || src[i] == '\r') { i++; }
        else if (src[i] == '\n') { i++; line++; 

        // } else if (starts_with(src, src_len, i, "let")) {
        //     Token token;
        //     token.type = "LET";
        //     token.value = NULL;
        //     token.atPos = i;
        //     token.atLine = line;
        //     push_token(&result, token);
        //     i += 3;

        } else if (starts_with(src, src_len, i, "=")) {
            Token token;
            token.type = "ASSIGN";
            token.value = NULL;
            token.atPos = i;
            token.atLine = line;
            push_token(result, token);
            i += 2;

        // Handle identifiers
        } else if (
            (src[i] >= 'a' && src[i] <= 'z') || 
            (src[i] >= 'A' && src[i] <= 'Z') 
        ) {
            size_t end = i;

            while (i < src_len && src[i] != '\0' && 
                (
                    (src[i] >= 'a' && src[i] <= 'z') || 
                    (src[i] >= 'A' && src[i] <= 'Z') ||
                    (src[i] == '_') ||
                    (src[i] >= '0' && src[i] <= '9')
                )
            ) {
               i++; 
            }

            char* buffer;
            buffer = calloc(i - end + 1, sizeof(char));
            if (buffer == NULL) { perror("calloc failed"); };
            strncpy(buffer, src + end, i - end);
            buffer[i - end] = '\0';

            Token token;
            if (strcmp(buffer, "let") == 0) {
                token.type = "LET";
                token.value = NULL;
            } else {
                token.type = "ID";
                token.value = buffer;
            }
            token.atPos = end;
            token.atLine = line;
            push_token(result, token);

        // handle ints, decimals & scientific notations
        } else if (
            (src[i] >= '0' && src[i] <= '9')
        ) {
            Token token;
            token.type = "INT_LIT";
            token.atPos = i;
            token.atLine = line;

            size_t start = i;
            i++;

            while (i < src_len && src[i] != '\0' && 
                (
                    (src[i] >= '0' && src[i] <= '9') || 
                    (src[i] == '_') ||
                    (src[i] == '.')
                )
            ) {
                if (src[i] == '_') {
                    i++;
                }
                if (src[i] == '.') {
                    if (token.type == "FLOAT_LIT") {
                        break;
                    } else {
                        token.type = "FLOAT_LIT";
                    }
                }
                i++;
            }

            char* buffer;
            buffer = calloc(i - start + 1, sizeof(char));
            if (buffer == NULL) { perror("calloc failed"); };

            // strncpy(buffer, src + start, i - start);
            // copy from src to buffer, char by char, skipping '_'
            size_t j = 0;
            for (size_t k = start; k < i; k++) {
                if (src[k] == '_') { continue; }
                buffer[j] = src[k];
                j++;
            }

            buffer[i - start] = '\0';

            token.value = buffer;
            push_token(result, token);
          
        } else {
            // Raise Error
            Compiler_Error error;
            error.type = "LEX_ERROR";
            error.atPos = i;
            error.atLine = line;
            
            // iterate until next whitespace
            size_t start = i;
            while (
                i < src_len && 
                src[i] != '\0' && 
                src[i] != ' ' && 
                src[i] != '\t' && 
                src[i] != '\n'
            ) {
                if (src[i] == '\n') { line++; }    
                i++;
            }

            error.message = "Invalid token found -> ";
            
            char* buffer;
            buffer = calloc(i - start + 1, sizeof(char));
            if (buffer == NULL) { perror("calloc failed"); };

            // strncpy(buffer, src + start, i - start);
            // copy from src to buffer, char by char, skipping '_'
            size_t j = 0;
            for (size_t k = start; k < i; k++) {
                buffer[j] = src[k];
                j++;
            }

            buffer[i - start] = '\0';

            // Append invalid value in buffer to error.message
            error.message = strcat(error.message, buffer);
            push_error(result, error);
        }
    }  

    return errors_list;
};
