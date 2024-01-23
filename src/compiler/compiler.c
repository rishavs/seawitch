#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "compiler.h"

void free_tokens(ListOfTokens* tokens) {
    c_foreach(i, ListOfTokens, *tokens) {
        cstr_drop(&i.ref->kind);
        cstr_drop(&i.ref->value);
    }
    ListOfTokens_drop(tokens);
}

int compile_file (cstr filepath) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    cstr src = read_file (filepath);
    printf("Source code:\n---------------------------------\n");
    printf("%s\n", cstr_str(&src));

    ListOfTokens tokens = lex_file(filepath);
    printf("Tokens:\n---------------------------------\n");
    c_foreach (i, ListOfTokens, tokens)  {   // Alternative and recommended way to iterate.
        printf("%s, %s \n", cstr_str(&i.ref->kind), cstr_str(&i.ref->value));      // i.ref is a pointer to the current element.
    }

    cstr_drop(&src);
    free_tokens(&tokens);
        
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time Taken:\n---------------------------------\n");
    printf("Compilation took %f seconds to execute \n", cpu_time_used);
        
    return 0;
}