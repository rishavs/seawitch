#ifndef SEAWITCH_H
#define SEAWITCH_H

typedef struct Token
{
    enum
    {
        TOK_EOF ,
        TOK_ILLEGAL,
        TOK_SPACE,
        TOK_VAR,
        TOK_CONST,
        TOK_ASSIGN,
        TOK_INT_LIT,
        TOK_FLOAT_LIT,
        TOK_BOOL_LIT,
        TOK_IDENTIFIER,
    } type;
    char *value;
    fpos_t cursor;
    size_t atPos;
    size_t atLine;
} Token;

typedef struct ParsingContext {
    FILE *fp;
    size_t pos;
    size_t line;
    fpos_t cursor;
} ParsingContext;

void parse (const char* filePath);



#endif // SEAWITCH_H