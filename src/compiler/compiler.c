#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../include/ast.h"
#include "../include/compiler.h"

int compile_file(char* filepath) {
    // Calculate the time taken by fun()
    clock_t t;
    t = clock();
   
    // -------------------------------
    // Read File 
    // -------------------------------   
    char* src = read_file(filepath); 
    printf("1. Reading File: %s\n----\n%s\n----\n", filepath, src);

    t = clock() - t;
    double readingDuration = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nTime Taken for Reading File = %f\n", readingDuration);
    
    // -------------------------------
    // Lex Source
    // -------------------------------
    Lexing_Result lexer = lex_file(src);
    puts("----");
    printf("2. Lexing Source:\n----\n[\n");
    printf("num tokens = %d\n", lexer.tok_count);
    for (size_t i = 0; i < lexer.tok_count; i++) {
        // if (tokens[i].type == 0) {break;};
        printf("    { type: %s, value: %s, atPos: %d, atLine: %d },\n", 
            lexer.tokens[i].type, 
            lexer.tokens[i].value, 
            lexer.tokens[i].atPos, 
            lexer.tokens[i].atLine
        );
    }
    printf("num errors = %d\n", lexer.err_count);
    for (size_t i = 0; i < lexer.err_count; i++) {
        // if (tokens[i].type == 0) {break;};
        printf("    { type: %s, message: %s, atPos: %d, atLine: %d },\n", 
            lexer.errors[i].type, 
            lexer.errors[i].message, 
            lexer.errors[i].atPos, 
            lexer.errors[i].atLine
        );
    }

    // printf("\n]\nTime Taken = %d", ->duration);
    t = clock() - t;
    double lexingDuration = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\n]\nTime Taken for Lexing source = %f", lexingDuration);
    // free(tokens);

    // -------------------------------
    // Parse Tokens
    // -------------------------------   
    Parsing_Result parser = parse_file(&lexer);
    print_ast(parser.ast);
    // Parse tokens into AST
    // AST* ast = parse(tokens);
    // puts("----");
    // printf("3. Parsing Tokens:\n----\n[\n");
    // printf("num ast nodes = %d\n", sizeof(ast));
    // for (size_t i = 0; i < sizeof(ast); i++) {
    //     // if (ast[i].type == 0) {break;};
    //     printf("    { type: %d, value: %s, atPos: %d, atLine: %d },\n", 
    //         ast[i].type, 
    //         ast[i].value, 
    //         ast[i].atPos,
    return 0;
}