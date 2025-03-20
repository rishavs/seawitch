#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sw_bootstrap.h"
#include "sw_compiler.h"
#include "seawitch.h"

bool substring_at(char* src, size_t start, size_t end, char* frag) {    
    size_t frag_len = strlen(frag);
    if (end - start != frag_len) { return false; }
    for (size_t i = start; i < end; i++) {
        if (src[i] != frag[i - start]) { return false; }
    }
    return true;
}

void lex_code(Compiler_context* ctx) {

    size_t i = 0;
    size_t line = 0;
    size_t col = 0;

    ctx->nodes_len = 0;
    
    while (i < ctx->src_len && ctx->src[i] != '\0') {
        // Handle whitespace
        if (ctx->src[i] == ' ' || ctx->src[i] == '\t' || ctx->src[i] == '\r') { 
            i++; col++;
        } else if (ctx->src[i] == '\n') { 
            i++; line++; col = 0;

        // Handle operators
        } else if (substring_at(ctx->src, i, i + 1, "=")) {
            ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_ASSIGN;
            ctx->nodes_list[ctx->nodes_len].line = line;
            ctx->nodes_list[ctx->nodes_len].column = col;
            ctx->nodes_list[ctx->nodes_len].start = i;
            ctx->nodes_list[ctx->nodes_len].end = i + 1;

            i += 1; col += 1;
            ctx->nodes_len++; 

        // Handle identifiers
        } else if (
            (ctx->src[i] >= 'a' && ctx->src[i] <= 'z') || 
            (ctx->src[i] >= 'A' && ctx->src[i] <= 'Z') ||
            (ctx->src[i] == '_')
        ) {        
            ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_IDENTIFIER;
            ctx->nodes_list[ctx->nodes_len].line = line;
            ctx->nodes_list[ctx->nodes_len].column = col;
            ctx->nodes_list[ctx->nodes_len].start = i;

            while (
                i < ctx->src_len && ctx->src[i] != '\0' && 
                (
                    (ctx->src[i] >= 'a' && ctx->src[i] <= 'z') || 
                    (ctx->src[i] >= 'A' && ctx->src[i] <= 'Z') ||
                    (ctx->src[i] == '_') ||
                    (ctx->src[i] >= '0' && ctx->src[i] <= '9')
                )
            ) {
               i++; col++;
            }
            ctx->nodes_list[ctx->nodes_len].end = i;
            
            if (substring_at(ctx->src, ctx->nodes_list[ctx->nodes_len].start, ctx->nodes_list[ctx->nodes_len].end, "var")) {
                ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_VAR;
            } else {
                ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_IDENTIFIER;
            }
            ctx->nodes_len++;

        // handle ints, decimals & scientific notations
        } else if (
            (ctx->src[i] >= '0' && ctx->src[i] <= '9')
        ) {
            ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_INTEGER;
            ctx->nodes_list[ctx->nodes_len].line = line;
            ctx->nodes_list[ctx->nodes_len].column = col;
            ctx->nodes_list[ctx->nodes_len].start = i; 
            while (i < ctx->src_len && ctx->src[i] != '\0' && 
                (
                    (ctx->src[i] >= '0' && ctx->src[i] <= '9') || 
                    (ctx->src[i] == '_') ||
                    (ctx->src[i] == '.')
                )
            ) {
                if (ctx->src[i] == '.') {
                    if (ctx->nodes_list[ctx->nodes_len].token_kind == TOKEN_DECIMAL) {
                        break;
                    } else {
                        ctx->nodes_list[ctx->nodes_len].token_kind = TOKEN_DECIMAL;
                    }
                }
                i++; col++;
            }
            ctx->nodes_list[ctx->nodes_len].end = i;
            
            ctx->nodes_len++;
          
        } else {
            // Raise Error

            ctx->errors_list[ctx->errors_len].line = line;
            ctx->errors_list[ctx->errors_len].column = col;
            ctx->errors_list[ctx->errors_len].filepath = ctx->filepath;

            ctx->errors_list[ctx->errors_len].name = strict_calloc(64, sizeof(char), __FILE__, __LINE__);
            strcpy(ctx->errors_list[ctx->errors_len].name, "Syntax Error");

            ctx->errors_list[ctx->errors_len].message = strict_calloc(64, sizeof(char), __FILE__, __LINE__);
            sprintf(ctx->errors_list[ctx->errors_len].message, "Illegal character '%c'", ctx->src[i]);
            
            ctx->errors_len++;

            // skip chracters until next whitespace or newline
            while (
                i < ctx->src_len 
                && ctx->src[i] != '\0' 
                && ctx->src[i] != ' ' 
                && ctx->src[i] != '\n'
                && ctx->src[i] != '\t'
            ) {
                i++; col++;
            }
        }
    }  

}