
#include "compiler.h"

ListOfTokens lex_file (cstr src) {
    ListOfTokens tokens = {0};
    // lets create a dummy token
    Token tok1;
    tok1.kind = cstr_from("dummy_kind");
    tok1.value = cstr_from("dummy_value");
    tok1.pos = 0;
    tok1.line = 0;
    // cstr_starts_with(const cstr* self, const char* str);

    // lets iterate over the cstr using a for loop
    for 

    ListOfTokens_push(&tokens, tok1);
    // cstr token = cstr_new();
    // int i = 0;
    // while (i < source.len) {
    //     char c = source.str[i];
    //     if (c == ' ' || c == '\n' || c == '\t') {
    //         if (token.len > 0) {
    //             Token tok = Token_init(token);
    //             ListOfTokens_push(&tokens, tok);
    //             token = cstr_new();
    //         }
    //     } else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']') {
    //         if (token.len > 0) {
    //             Token tok = Token_init(token);
    //             ListOfTokens_push(&tokens, tok);
    //             token = cstr_new();
    //         }
    //         cstr_push(&token, c);
    //         Token tok = Token_init(token);
    //         ListOfTokens_push(&tokens, tok);
    //         token = cstr_new();
    //     } else if (c == '"') {
    //         if (token.len > 0) {
    //             Token tok = Token_init(token);
    //             ListOfTokens_push(&tokens, tok);
    //             token = cstr_new();
    //         }
    //         cstr_push(&token, c);
    //         i++;
    //         while (i < source.len && source.str[i] != '"') {
    //             cstr_push(&token, source.str[i]);
    //             i++;
    //         }
    //         cstr_push(&token, '"');
    //         Token tok = Token_init(token);
    //         ListOfTokens_push(&tokens, tok);
    //         token = cstr_new();
    //     } else {
    //         cstr_push(&token, c);
    //     }
    //     i++;
    // }
    // if (token.len > 0) {
    //     Token tok = Token_init(token);
    //     ListOfTokens_push(&tokens, tok);
    //     token = cstr_new();
    // }
    return tokens;
}