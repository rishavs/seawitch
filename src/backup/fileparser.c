#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hexal.h"


// parse_file
int parse_file(Hx_File_Parsing_Context* ctx, Hx_Compiler_Metadata* meta) {
    printf("Parsing file: %s\n", ctx->fp);

    while {
        advance_tokens(ctx, meta);
        if (ctx->curr_token.type == TOK_EOF) {
            break;
        }
    }
}




// parse_var_decl