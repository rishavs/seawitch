#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sw_system.h"
#include "sw_string.h"
#include "seawitch.h"
#include "compiler.h"

bool substring_at(DynString* src, size_t start, size_t end, char* frag) {    
    size_t frag_len = strlen(frag);
    if (
        src == NULL || frag == NULL || frag_len == 0 || 
        start < 0 || start > end || end > src->len
    ) { return false; }
    if (end - start != frag_len) { return false; }
    for (size_t i = start; i < end; i++) {
        if (src->data[i] != frag[i - start]) { return false; }
    }
    return true;
}

void transpiler_ctx_do_push_token (Transpiler_context* ctx, Token token) {
    if (ctx->token_len >= ctx->token_capacity) {
        ctx->token_capacity *= 2;
        ctx->tokens_list = strict_realloc(ctx->tokens_list, ctx->token_capacity * sizeof(Token), __FILE__, __LINE__);
    }
    ctx->tokens_list[ctx->token_len] = token;
    ctx->token_len++;
}

void lex_file(Transpiler_context* ctx) {

    int i = 0;
    int line = 0;
    
    while (i < ctx->src->len && src[i] != '\0') {
        // Handle whitespace
        if (src[i] == ' ' || src[i] == '\t' || src[i] == '\r') { i++; }
        else if (src[i] == '\n') { i++; line++; 

        // Handle operators
        } else if (substring_at(ctx->src, i, i + 1, "=")) {
            Token token;
            token.type = TOKEN_ASSIGN;
            token.start = i;
            token.end = i + 1;
            token.line = line;
            token.column = i;
            transpiler_ctx_do_push_token(ctx, token);
            i += 1;

        // Handle identifiers
        } else if (
            (src[i] >= 'a' && src[i] <= 'z') || 
            (src[i] >= 'A' && src[i] <= 'Z') ||
            (src[i] == '_')
        ) {
            size_t end = i;

            while (i < ctx->src->len && src[i] != '\0' && 
                (
                    (src[i] >= 'a' && src[i] <= 'z') || 
                    (src[i] >= 'A' && src[i] <= 'Z') ||
                    (src[i] == '_') ||
                    (src[i] >= '0' && src[i] <= '9')
                )
            ) {
               end++; 
            }

            Token token;
            token.start = i;
            token.end = end;
            token.line = line;
            token.column = i;

            if (substring_at(ctx->src, i, end, "var")) {
                token.type = TOKEN_VAR;
            } else {
                token.type = TOKEN_IDENTIFIER;
            }

            transpiler_ctx_do_push_token(ctx, token);
            i = end;

        // handle ints, decimals & scientific notations
        } else if (
            (src[i] >= '0' && src[i] <= '9')
        ) {
            Token token;
            token.type = TOKEN_INTEGER;
            token.start = i;
            token.line = line;
            token.column = i;

            size_t end = i;

            while (i < ctx->src->len && src[i] != '\0' && 
                (
                    (src[i] >= '0' && src[i] <= '9') || 
                    (src[i] == '_') ||
                    (src[i] == '.')
                )
            ) {
                if (src[i] == '.') {
                    if (token.type == TOKEN_FLOAT) {
                        break;
                    } else {
                        token.type = TOKEN_INTEGER;
                    }
                }
                end++;
            }

            transpiler_ctx_do_push_token(ctx, token);
            i = end;
          
        } else {
            // Raise Error
            transpiler_ctx_do_push_error(ctx, snitch("Unknown token", __FILE__, __LINE__));
        }
    }  

};
