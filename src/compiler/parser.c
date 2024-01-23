#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "compiler.h"




Token* getNextToken(ParsingContext* ctx) {
    // read the next token from the file
    Token* token = malloc(sizeof(Token));
    char kind[256]; // temporary buffer
    if (fscanf(ctx->file, "%255s", kind) == EOF) {
        free(token);
        return NULL;
    }
    token->kind = cstr_from(kind);
    return token;
}

void advanceParser(ParsingContext* ctx) {
    // free the memory of the oldest token
    if (ctx->prev2ndToken != NULL) {
        cstr_drop(&ctx->prev2ndToken->kind);
        free(ctx->prev2ndToken);
    }

    // shift the tokens
    ctx->prev2ndToken = ctx->prev1stToken;
    ctx->prev1stToken = ctx->currToken;
    ctx->currToken = ctx->next1stToken;
    ctx->next1stToken = ctx->next2ndToken;

    // get the next token
    ctx->next2ndToken = getNextToken(ctx);
}

void parse_file (cstr filepath) {
    // create the context
    ParsingContext* ctx = malloc(sizeof(ParsingContext));
    ctx->file = fopen(cstr_str(&filepath), "r");
    if (ctx->file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // initialize the tokens
    ctx->prev2ndToken = NULL;
    ctx->prev1stToken = NULL;
    ctx->currToken = NULL;
    ctx->next1stToken = getNextToken(ctx);
    ctx->next2ndToken = getNextToken(ctx);

    // print all tokens until EOF is reached
    while (ctx->next1stToken != NULL) {
        printf("%s\n", cstr_str(&ctx->next1stToken->kind));
        advanceParser(ctx);
    }

    // clean up
    fclose(ctx->file);
    free(ctx);
}