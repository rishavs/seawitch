#include "common.h"
#include "helpers.h"

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
void add_token(List* tokens, char* kind, char* value, size_t i, int line) {
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
            // roll up all spaces into a single SPACE token
            while (is_space(src[i]) || src[i] == '\n') {
                if (src[i] == '\n') {
                    line++;
                }
                i++;
            }
            add_token(tokens, "SPACE", NULL, i, line);
        // ----------------------------------------
        // Handle Comments
        // ----------------------------------------
        } else if (matches(src, i, "--")) {
            add_token(tokens, "COMMENT_START", NULL, i, line);
            i += 2;
            while (src[i] != '\0') {
                if (matches(src, i, "--")) {
                    add_token(tokens, "COMMENT_END", NULL,  i, line);
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
            add_token(tokens, "VAR", NULL, i, line);
            i += 3;

        // ----------------------------------------
        // Handle Operators
        // ----------------------------------------
		} else if (matches(src, i, "==")) {
            add_token(tokens, "EQUALS", NULL, i, line);
			i += 2;
		} else if (matches(src, i, "=")) {
            add_token(tokens, "ASSIGN", NULL, i, line);
			i++;

	    // ----------------------------------------
        // Handle INTEGERS, FLOATS and EXPONENTIALS
        // ----------------------------------------
		} else if (is_digit(src[i])) {
            Buffer* buffer = create_buffer(2);
            add_to_buffer(buffer, src[i]);
            i++;

            bool is_float = false;
            bool is_exp = false;

            while (is_digit(src[i]) || src[i] == '.' || src[i] == 'e' || src[i] == '_') {
                if (src[i] == '_') {
					i++;
				} else if (is_digit(src[i])) {
                    add_to_buffer(buffer, src[i]);
                    i++;
				} else if (src[i] == '.' && !is_float) {
					is_float = true;
                    add_to_buffer(buffer, src[i]);
                    i++;
				} else if (src[i] == 'e' && !is_exp) {
					is_exp = true;
                    add_to_buffer(buffer, src[i]);
                    i++;
                    while (is_digit(src[i])) {
                        add_to_buffer(buffer, src[i]);
                        i++;
                    };
				} else {
					break;
				};
            }

            close_buffer(buffer);

			if (is_float) {
                add_token(tokens, "FLOAT_LIT", buffer->data, i, line);
			} else if (is_exp) {
                add_token(tokens, "EXP_LIT", buffer->data, i, line);
			} else {
                add_token(tokens, "INT_LIT", buffer->data, i, line);
			};
            free(buffer);

        // ----------------------------------------
        // Handle Identifiers
        // ----------------------------------------
		} else if (is_alphabet(src[i]) || src[i] == '_') {
            Buffer* buffer = create_buffer(2);
            add_to_buffer(buffer, src[i]);
            i++;

            while (src[i] != '\0') {
                if (is_alphanum(src[i]) || src[i] == '_') {
                    add_to_buffer(buffer, src[i]);
                    i += 1;
                } else {
                    break;
                }
            }

            close_buffer(buffer);
            add_token(tokens, "IDENT", buffer->data, i, line);
            free(buffer);

        // ----------------------------------------
        // Handle ILLEGALS
        // ----------------------------------------
		} else {
            add_token(tokens, "ILLEGAL", (char[2]){src[i], '\0'}, i, line);
			i++;
        };
    }

    add_token(tokens, "EOF", NULL, i, line);
    
    return tokens;
}
