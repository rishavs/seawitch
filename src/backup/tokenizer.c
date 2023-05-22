#include "hexal.h"

// the advance_tokens functions takes in the context and metadata object and advances the tokens

Token get_next_token(Hx_File_Parsing_Context ctx, Hx_Compiler_Metadata metadata) {
    // get the next token

    
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
};

int advance_tokens(Hx_File_Parsing_Context* ctx, Hx_Compiler_Metadata* metadata) {
    // advance the tokens
    ctx->prev_token = ctx->curr_token;
    ctx->curr_token = ctx->next_token;
    ctx->next_token = get_next_token(ctx->fp, ctx->cursor, ctx->pos, ctx->line);
    // check if the next token is a newline
    if (ctx->next_token.type == NEWLINE) {
        ctx->line++;
        ctx->pos = 0;
    } else {
        ctx->pos++;
    }
    return 0;
}

