#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../include/ast.h"
#include "../include/compiler.h"

void push_error(Compiler_Errors_List* errors , Compiler_Error error) {
    if (errors->count >= errors->capacity) {
        errors->capacity *= 2;
        errors->errors = realloc(errors->errors, errors->capacity * sizeof(Compiler_Error));
        if (errors->errors == NULL) { perror("realloc failed"); };
    }
    errors->errors[errors->count] = error;
    errors->count++;
}

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
    Tokens_List tokens_list;
    Compiler_Errors_List lex_errors = lex_file(&tokens_list, src);
    if (lex_errors.count > 0) {
        puts("Lexing failed");
    }

    puts("----");
    printf("2. Lexing Source:\n----\n[\n");
    printf("num tokens = %d\n", tokens_list.count);
    for (size_t i = 0; i < tokens_list.count; i++) {
        // if (tokens[i].type == 0) {break;};
        printf("    { type: %s, value: %s, atPos: %d, atLine: %d },\n", 
            tokens_list.tokens[i].type, 
            tokens_list.tokens[i].value, 
            tokens_list.tokens[i].atPos, 
            tokens_list.tokens[i].atLine
        );
    }
    printf("num errors = %d\n", lex_errors.count);
    for (size_t i = 0; i < lex_errors.count; i++) {
        // if (tokens[i].type == 0) {break;};
        printf("    { type: %s, message: %s, atPos: %d, atLine: %d },\n", 
            lex_errors.errors[i].type, 
            lex_errors.errors[i].message, 
            lex_errors.errors[i].atPos, 
            lex_errors.errors[i].atLine
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
    Leaf ast;
    Compiler_Errors_List parser_errors = parse_file(&ast, &tokens_list);
    if (parser_errors.count > 0) {
        puts("Parsing failed");
    }
    print_ast(&ast);
    // Parsing skips if ILLEGAL token found or if enclosures are not found before end of file
    // But till then parsing should happen and return all errors

    // Parsing_Result parser = parse_file(&lexer);
    // print_ast(parser.ast);
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