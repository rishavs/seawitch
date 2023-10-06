#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compiler.h"

bool is_space(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r';
}

bool is_alphabet(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool is_digit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool is_alphanum(char ch) {
    return is_alphabet(ch) || is_digit(ch);
}

bool matches(const char* str, size_t i, const char* frag) {
    size_t j = 0;
    while (frag[j] != '\0') {
        if (str[i + j] != frag[j]) {
            return false;
        }
        j++;
    }
    return true;
}

// Function to create a new Hx_Token dynamically
void add_token(List* tokens, Hx_Token_Kind kind, char* value, size_t i, int line) {
    Hx_Token* token = malloc(sizeof(Hx_Token));
    if (token == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    token->kind = kind;
    token->value = value;
    token->pos = i;
    token->line = line;

    list_push(tokens, token);
}


// lexer takes a string as input and returns a list of tokens
List* lex_file(char* src) {

    size_t i = 0;
    int line = 1;
    
    // Create a new list to hold Hx_Token pointers
    List* tokens = list_new(1); // Initialize with an initial capacity (e.g., 10)

    // Loop through the source code string
    while (src[i] != '\0') {

		// ----------------------------------------
		// Handle Spaces
		// ----------------------------------------
        if (is_space(src[i]) || src[i] == '\n') {
            i++;
            // roll up all spaces into a single SPACE token
            while (is_space(src[i]) || src[i] == '\n') {
                if (src[i] == '\n') {
                    line++;
                }
                i++;
            }
            add_token(tokens, TOK_SPACE, NULL, i, line);
        // ----------------------------------------
        // Handle Comments
        // ----------------------------------------
        } else if (matches(src, i, "--")) {
            add_token(tokens, TOK_COMMENT_START, NULL, i, line);
            i += 2;
            while (src[i] != '\0') {
                if (matches(src, i, "--")) {
                    add_token(tokens, TOK_COMMENT_END, NULL,  i, line);
                    i += 2;
                    break;
                } else if (src[i] == '\n') {
                    i++;
                    line++;
                } else {
                    i++;
                }
            }

        // ----------------------------------------
        // Handle all Keywords
        // ----------------------------------------
		} else if (matches(src, i, "var")){
            add_token(tokens, TOK_VAR, NULL, i, line);
            i += 3;

        // ----------------------------------------
        // Handle ILLEGALS
        // ----------------------------------------
		} else {
            add_token(tokens, TOK_ILLEGAL, (char[2]){src[i], '\0'}, i, line);
			i++;
        };
    }

    add_token(tokens, TOK_EOF, NULL, i, line);
    
    return tokens;
}
