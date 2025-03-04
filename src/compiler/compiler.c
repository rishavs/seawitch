#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sw_core.h"

#include "seawitch.h"
#include "sw_compiler.h"

Transpiler_context* transpiler_ctx_do_init (char* filepath, char* src) {
    Transpiler_context* ctx = strict_calloc(1, sizeof(Transpiler_context), __FILE__, __LINE__);
    ctx->error_capacity = 8;
    ctx->error_len = 0;
    ctx->error_refs_list = strict_calloc(ctx->error_capacity, sizeof(Compiler_error), __FILE__, __LINE__);

    ctx->filepath = filepath;
    ctx->filepath_len = strlen(filepath);
    ctx->src = src;
    ctx->src_len = strlen(src);
    ctx->reading_duration = 0;
    
    ctx->token_capacity = 8;
    ctx->token_len = 0;
    ctx->tokens_list = strict_calloc(ctx->token_capacity, sizeof(Token), __FILE__, __LINE__);
    ctx->lexing_duration = 0;

    ctx->ast = strict_calloc(1, sizeof(ASTNode), __FILE__, __LINE__);
    ctx->ast->kind = ASTNODE_PROGRAM;
    ctx->ast->props.Program.filepath = 
    ctx->parsing_duration = 0;

    return ctx;
}

void transpiler_ctx_do_push_error (Transpiler_context* ctx, Compiler_error* err) {
    if (ctx->error_len >= ctx->error_capacity) {
        ctx->error_capacity *= 2;
        ctx->error_refs_list = strict_realloc(ctx->error_refs_list, ctx->error_capacity * sizeof(Compiler_error*), __FILE__, __LINE__);
    }
    ctx->error_refs_list[ctx->error_len] = err;
    ctx->error_len++;
}

void print_tokens(Transpiler_context* ctx) {
    puts("----");
    printf("Lexer: (num tokens = %zu)\n----\n[\n", ctx->token_len);
    for (size_t i = 0; i < ctx->token_len; i++) {
        printf("\t");
        for (size_t j = ctx->tokens_list[i].start; j < ctx->tokens_list[i].end; j++) {
            printf("%c", ctx->src[j]);
        }
        printf("\t%s, \tline: %zu, \tcol: %zu,\n",
            Token_kind_strings[ctx->tokens_list[i].kind],
            ctx->tokens_list[i].line, 
            ctx->tokens_list[i].column
        );
    }
}

void transpile_code (Transpiler_context* ctx) {
    // Start the clock
    clock_t t;
    t = clock();

    // -------------------------------
    // Lex Source
    // -------------------------------
    transpiler_ctx_do_push_error(ctx, snitch("TODO", __FILE__, __LINE__));

    printf("Reader: (File: %s)\n----\n%s\n", ctx->filepath, ctx->src);
    lex_code(ctx);
    print_tokens(ctx);

    t = clock() - t;
    ctx->lexing_duration = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\n]\nTime Taken for Lexing source = %f", ctx->lexing_duration);

    // -------------------------------
    // Parse Tokens
    // -------------------------------
    parse_code(ctx);
}


// #include "../include/ast.h"
// #include "../include/compiler.h"

// void push_error(Compiler_Errors_List* errors , Compiler_Error error) {
//     if (errors->count >= errors->capacity) {
//         errors->capacity *= 2;
//         errors->errors = realloc(errors->errors, errors->capacity * sizeof(Compiler_Error));
//         if (errors->errors == NULL) { perror("realloc failed"); };
//     }
//     errors->errors[errors->count] = error;
//     errors->count++;
// }


// int compile_file(char* filepath) {
//     // Calculate the time taken by fun()
//     clock_t t;
//     t = clock();

  
//     // -------------------------------
//     // Read File 
//     // -------------------------------   
//     char* src = read_file(filepath); 
//     printf("1. Reading File: %s\n----\n%s\n----\n", filepath, src);

//     t = clock() - t;
//     double readingDuration = ((double)t)/CLOCKS_PER_SEC; // in seconds
//     printf("\nTime Taken for Reading File = %f\n", readingDuration);
    
//     // -------------------------------
//     // Lex Source
//     // -------------------------------
//     Tokens_List tokens_list;
//     Compiler_Errors_List lex_errors = lex_code(&tokens_list, src);
//     if (lex_errors.count > 0) {
//         puts("Lexing failed");
//     }

//     puts("----");
//     printf("2. Lexing Source:\n----\n[\n");
//     printf("num tokens = %d\n", tokens_list.count);
//     for (size_t i = 0; i < tokens_list.count; i++) {
//         // if (tokens[i].type == 0) {break;};
//         printf("    { type: %s, value: %s, atPos: %d, atLine: %d },\n", 
//             tokens_list.tokens[i].type, 
//             tokens_list.tokens[i].value, 
//             tokens_list.tokens[i].atPos, 
//             tokens_list.tokens[i].atLine
//         );
//     }
//     printf("num errors = %d\n", lex_errors.count);
//     for (size_t i = 0; i < lex_errors.count; i++) {
//         // if (tokens[i].type == 0) {break;};
//         printf("    { type: %s, message: %s, atPos: %d, atLine: %d },\n", 
//             lex_errors.errors[i].type, 
//             lex_errors.errors[i].message, 
//             lex_errors.errors[i].atPos, 
//             lex_errors.errors[i].atLine
//         );
//     }

//     // printf("\n]\nTime Taken = %d", ->duration);
//     t = clock() - t;
//     double lexingDuration = ((double)t)/CLOCKS_PER_SEC; // in seconds
//     printf("\n]\nTime Taken for Lexing source = %f", lexingDuration);
//     // free(tokens);

//     // -------------------------------
//     // Parse Tokens
//     // -------------------------------   
//     Leaf ast;
//     Compiler_Errors_List parser_errors = parse_file(&ast, &tokens_list);
//     if (parser_errors.count > 0) {
//         puts("Parsing failed");
//     }
//     print_ast(&ast);
//     // Parsing skips if ILLEGAL token found or if enclosures are not found before end of file
//     // But till then parsing should happen and return all errors

//     // Parsing_Result parser = parse_file(&lexer);
//     // print_ast(parser.ast);
//     // Parse tokens into AST
//     // AST* ast = parse(tokens);
//     // puts("----");
//     // printf("3. Parsing Tokens:\n----\n[\n");
//     // printf("num ast nodes = %d\n", sizeof(ast));
//     // for (size_t i = 0; i < sizeof(ast); i++) {
//     //     // if (ast[i].type == 0) {break;};
//     //     printf("    { type: %d, value: %s, atPos: %d, atLine: %d },\n", 
//     //         ast[i].type, 
//     //         ast[i].value, 
//     //         ast[i].atPos,
//     return 0;
// }