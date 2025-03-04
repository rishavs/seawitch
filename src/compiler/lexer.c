#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sw_core.h"
#include "seawitch.h"
#include "sw_compiler.h"

bool substring_at(char* src, size_t start, size_t end, char* frag) {    
    size_t frag_len = strlen(frag);
    if (end - start != frag_len) { return false; }
    for (size_t i = start; i < end; i++) {
        if (src[i] != frag[i - start]) { return false; }
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

void lex_code(Transpiler_context* ctx) {

    size_t i = 0;
    size_t line = 0;
    size_t col = 0;
    
    while (i < ctx->src_len && ctx->src[i] != '\0') {
        // Handle whitespace
        if (ctx->src[i] == ' ' || ctx->src[i] == '\t' || ctx->src[i] == '\r') { 
            i++; col++;
        } else if (ctx->src[i] == '\n') { 
            i++; line++; col = 0;

        // Handle operators
        } else if (substring_at(ctx->src, i, i + 1, "=")) {
            Token token;
            token.kind = TOKEN_ASSIGN;
            token.start = i;
            token.end = i + 1;
            token.line = line;
            token.column = col;
            transpiler_ctx_do_push_token(ctx, token);
            i += 1; col += 1;

        // Handle identifiers
        } else if (
            (ctx->src[i] >= 'a' && ctx->src[i] <= 'z') || 
            (ctx->src[i] >= 'A' && ctx->src[i] <= 'Z') ||
            (ctx->src[i] == '_')
        ) {            
            Token token;
            token.start = i;
            token.end = i;
            token.line = line;
            token.column = col;

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
            token.end = i;

            if (substring_at(ctx->src, token.start, token.end, "var")) {
                token.kind = TOKEN_VAR;
            } else {
                token.kind = TOKEN_IDENT;
            }

            transpiler_ctx_do_push_token(ctx, token);

        // handle ints, decimals & scientific notations
        } else if (
            (ctx->src[i] >= '0' && ctx->src[i] <= '9')
        ) {
            Token token;
            token.kind = TOKEN_INTEGER;
            token.start = i;
            token.end = i;
            token.line = line;
            token.column = col;

            while (i < ctx->src_len && ctx->src[i] != '\0' && 
                (
                    (ctx->src[i] >= '0' && ctx->src[i] <= '9') || 
                    (ctx->src[i] == '_') ||
                    (ctx->src[i] == '.')
                )
            ) {
                if (ctx->src[i] == '.') {
                    if (token.kind == TOKEN_DECIMAL) {
                        break;
                    } else {
                        token.kind = TOKEN_DECIMAL;
                    }
                }
                i++; col++;
            }
            token.end = i;
            transpiler_ctx_do_push_token(ctx, token);
          
        } else {
            // Raise Error
            transpiler_ctx_do_push_error(ctx, snitch("Unknown token", __FILE__, __LINE__));
            i++; col++;
        }
    }  

}