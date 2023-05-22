#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>

typedef struct Str {
    size_t capacity;                            // allocated size of buffer
    size_t length;                              // used size of buffer
    wchar_t* buffer;                            // character buffer

    void (*init)(struct Str*);                  // initialize with a string
    void (*push)(struct Str*, wchar_t);         // append a single character
    void (*trim)(struct Str*);                  // remove unused space
    void (*destroy)(struct Str*);               // free memory
} Str;

// append a single character
void push_Str(Str* str, wchar_t ch) {
    if (str->length + 1 >= str->capacity) {
        str->capacity = str->capacity * 2;
        str->buffer = (wchar_t*)realloc(str->buffer, (str->capacity + 1) * sizeof(wchar_t));
    }
    str->buffer[str->length++] = ch;
    str->buffer[str->length] = L'\0';
}

// append a string to original string
Str* concat_Str(Str *str1, const Str *str2) {
    size_t new_len = str1->length + str2->length;
    if (new_len >= str1->capacity) {
        str1->capacity = new_len * 2;
        str1->buffer = (wchar_t*) realloc(str1->buffer, str1->capacity * sizeof(wchar_t));
    }
    wcscpy(&str1->buffer[str1->length], str2->buffer);
    str1->length = new_len;
    return str1;
}

// remove unused space
void trim_Str(Str* str) {
    str->buffer = (wchar_t*)realloc(str->buffer, (str->length + 1) * sizeof(wchar_t));
    str->capacity = str->length;
}

// free memory
void destroy_Str(Str* str) {
    free(str->buffer);
}

// initialize as empty
void init_Str(Str* str) {
    str->capacity = 8;
    str->length = 0;
    str->buffer = (wchar_t*)malloc((str->capacity + 1) * sizeof(wchar_t));
    if (str->buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    str->buffer[0] = L'\0';

    str->init = &init_Str;
    str->push = &push_Str;
    str->trim = &trim_Str;
    str->destroy = &destroy_Str;
}

typedef struct Token
{
    enum
    {
        TOK_EOF = 1,
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
    Str value;
    // fpos_t atPos;
    // size_t atLine;
} Token;

void printToken(Token token) {
    printf("Printing Token: ");
    printf("{ type: %i, value: %ls }, ", token.type, token.value.buffer);
}

Token getNextToken(FILE *fp, size_t line) {
    printf("Getting next token\n");

    // get cursor position
    fpos_t cursor;
    fgetpos(fp, &cursor);

    // get the char at the position
    wchar_t ch;
    ch = fgetwc(fp);

    // create a buffer to hold the token value
Str buffer = {0};
buffer.init(&buffer);

    // buffer.push(&buffer, ch);

    // if we are at end of fie, return a EOF token
    if (ch == WEOF) {
        Token token;
        token.type = TOK_EOF;
        token.value = buffer;
        // token.atPos = cursor;
        // token.atLine = line;
        return token;
    }

    // Create a token to return
    Token token;
    token.type = TOK_ILLEGAL;
    token.value = buffer;
    // token.atPos = cursor;
    // token.atLine = line;

  

    printToken(token);

    if (iswspace(ch)) {

        token.type = TOK_SPACE;

        // roll all spaces into a single space token
        while (iswspace(ch)) {
            ch = fgetwc(fp);
            // if (ch == L'\n') {
            //     token.atLine++;
            // }
        }
        token.value.push(&token.value, ch);
    
    } else if (iswdigit(ch)) {
        token.type = TOK_INT_LIT;
        token.value.push(&token.value, ch);

        // check for float lit first
        bool isFloat = false;

        // roll all digits into a single float 
        while (iswdigit(ch) || ch == L'.' || ch == L'_') {
            // push to buffer
            token.value.push(&token.value, ch);

            ch = fgetwc(fp);
            if (ch == L'.' && !isFloat) {
                isFloat = true;
                token.type = TOK_FLOAT_LIT;
            } else if (ch == L'.' && isFloat) {
                token.type = TOK_FLOAT_LIT;
                break;
            }
        }
    }
    else if (iswalpha(ch)) {
        token.type = TOK_IDENTIFIER;
        token.value.push(&token.value, ch);

        // roll all alphanum into the buffer
        while (iswalpha(ch) || ch == L'_') {
            // push to buffer
            // buffer.push(&buffer, ch);
            token.value.push(&token.value, ch);

            // check for keywords
            if (wcscmp(token.value.buffer, L"var") == 0) {
                token.type = TOK_VAR;
            } else if (wcscmp(token.value.buffer, L"const") == 0) {
                token.type = TOK_CONST;
            } else if (wcscmp(token.value.buffer, L"true") == 0 || wcscmp(token.value.buffer, L"false") == 0) {
                token.type = TOK_BOOL_LIT;
            } 

            // check for the next character
            ch = fgetwc(fp);
        }
    }
    // else {

    // }
    printf("Token: ");
    printToken(token);
    return token;
}


void parseFile (const char* filePath) {
    
    FILE* fp;
    fopen_s(fp, filePath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        exit(1);
    }
    // set file to wide character mode
    if (fwide(fp, 1) < 0) {
        fprintf(stderr, "Failed to set file to wide character mode\n");
        fclose(fp);
        exit(1);
    }

    size_t line = 0;

    Token token1 = getNextToken(fp, line);
    // line = token1.atLine;
    printToken(token1);
    
    Token token2 = getNextToken(fp, line);
    // line = token2.atLine;
    printToken(token2);
    
    Token token3 = getNextToken(fp, line);
    // line = token3.atLine;
    printToken(token3);
    
    Token token4 = getNextToken(fp, line);
    // line = token4.atLine;
    printToken(token4);
    
    Token token5 = getNextToken(fp, line);
    // line = token5.atLine;
    printToken(token5);
    
    Token token6 = getNextToken(fp, line);
    // line = token6.atLine;
    printToken(token6);
    
    Token token7 = getNextToken(fp, line);
    printToken(token7);
    
    // return 0;
}

int main (void) {
    parseFile("input/test.hex");
    return 0;
}