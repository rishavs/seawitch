#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "seawitch.h"

void appedCharToStr(char* str, size_t len, size_t cap,  char ch) {
    if (len + 1 >= cap) {
        cap = cap * 2;
        str = (char*)realloc(str, (cap + 1) * sizeof(char));
    }
    str[len++] = ch;
    str[len] = '\0';
}

void appendCharToStr2(char** str, size_t* len, size_t* cap, char ch) {
    if (*len + 1 >= *cap) {
        *cap = (*cap == 0) ? 8 : (*cap * 2);
        *str = realloc(*str, (*cap + 1) * sizeof(char));
        if (!*str) {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    (*str)[(*len)++] = ch;
    (*str)[*len] = '\0';
}


Token getNextToken(ParsingContext* ctx) {
    
    int ch;
    ch = fgetc(ctx->fp);
    ctx->pos++;
    
    // setup buffer
    char *buffer;
    size_t bufferLen = 0;
    size_t bufferCap = 8;
    buffer = malloc(bufferCap * sizeof(char));

    // if we are at end of fie, return a EOF token
    if (ch == EOF) {
        return (struct Token) { TOK_EOF, NULL, ctx->cursor, ctx->pos, ctx->line };
    
    } else if (isspace(ch)) {
        // roll all spaces into a single space token
        while (isspace(ch)) {
            if (ch == '\n') {
                ctx->line++;
            }
        
            ch = fgetc(ctx->fp);
            ctx->pos++;
        }
        return (struct Token) { TOK_SPACE, NULL, ctx->cursor, ctx->pos, ctx->line };
    
    } else if (isdigit(ch)) {

        // check for float lit first
        bool isFloat = false;

        // // push to buffer
        // buffer[bufferLen] = ch;
        // buffer[bufferLen + 1] = '\0';
        // bufferLen += 2;

        // roll all digits into a single float 
        while (isdigit(ch) || ch == L'.' || ch == L'_') {
            // resize buffer, if needed
            if (bufferLen + 1 == bufferCap) {
                bufferCap *= 2;
                buffer = realloc(buffer, bufferCap * sizeof(char));
            };
            // push to buffer
            buffer[bufferLen] = ch;
            buffer[bufferLen + 1] = '\0';
            bufferLen += 2;

            ch = fgetc(ctx->fp);
            ctx->pos++;

            if (ch == L'.' && !isFloat) {
                isFloat = true;
            } else if (ch == L'.' && isFloat) {
                break;
            }
        }
        if (isFloat) {
            return (struct Token) { TOK_FLOAT_LIT, buffer, ctx->cursor, ctx->pos, ctx->line };
        } else {
            return (struct Token) { TOK_INT_LIT, buffer, ctx->cursor, ctx->pos, ctx->line };
        }
    }
    else if (isalpha(ch)) {

        // // push to buffer
        // buffer[bufferLen] = ch;
        // buffer[bufferLen + 1] = '\0';
        // bufferLen += 2;

        // roll all alphanum into the buffer
        while (isalpha(ch) || ch == L'_') {
            // resize buffer, if needed
            if (bufferLen == bufferCap) {
                bufferCap *= 2;
                buffer = realloc(buffer, bufferCap * sizeof(char));
            };
            // push to buffer
            buffer[bufferLen] = ch;
            buffer[bufferLen + 1] = '\0';
            bufferLen += 2;

            ch = fgetc(ctx->fp);
            ctx->pos++;

        }
        // check for keywords
        if (strcmp(buffer, "var") == 0) {
            return (struct Token) { TOK_VAR, buffer, ctx->cursor, ctx->pos, ctx->line };
        } else if (strcmp(buffer, "const") == 0) {
            return (struct Token) { TOK_CONST, buffer, ctx->cursor, ctx->pos, ctx->line };
        } else if (strcmp(buffer, "true") == 0 || strcmp(buffer, "false") == 0) {
            return (struct Token) { TOK_BOOL_LIT, buffer, ctx->cursor, ctx->pos, ctx->line };
        } 
    }
    return (struct Token) { TOK_ILLEGAL, buffer, ctx->cursor, ctx->pos, ctx->line };
}

void parse (const char* filePath) {
    printf("Parsing file: %s\n", filePath);

    FILE *fp;
    fp = fopen(filePath, "r"); // Open the file for reading
    if (fp == NULL) {
        printf("Error: Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    ParsingContext ctx = { fp, 0, 0, 0 };

    // for (int i = 0; i < 10; i++) {
    Token tok;
    do {
        tok = getNextToken(&ctx);
        printf("{ type: %i, value: %ls, pos: %i, line: %i },\n", tok.type, tok.value, tok.atPos, tok.atLine);
    } while (tok.type != TOK_EOF);

    fclose(fp); // Close the file

}