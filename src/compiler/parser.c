#include <stdlib.h>

#include "sw_core.h"

#include "seawitch.h"
#include "sw_compiler.h"

// Forward declaration
ASTNode* parse_expression(Transpiler_context* ctx, size_t* i, Compiler_error* err);

ASTNode* parse_integer (Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    Token* token = ctx->tokens_list[i];
    ASTNode* int_node = strict_calloc(sizeof(ASTNode), __FILE__, __LINE__);
    int_node->kind      = ASTNODE_INTEGER;

    int_node->start     = token->start;
    int_node->end       = token->end;
    int_node->line      = token->line;
    int_node->column    = token->column;

    i++;
    return int_node;
};

ASTNode* parse_identifier (Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    Token* token = ctx->tokens_list[i];
    ASTNode* ident_node = strict_calloc(sizeof(ASTNode), __FILE__, __LINE__);
    ident_node->kind      = ASTNODE_IDENTIFIER;

    ident_node->start     = token->start;
    ident_node->end       = token->end;
    ident_node->line      = token->line;
    ident_node->column    = token->column;

    i++;
    return ident_node;
};

ASTNode* parse_primary_expression (Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    Token* token = ctx->tokens_list[i];

    switch (token->kind) {
    case TOKEN_INTEGER:
        return parse_integer(ctx, i, err);
        break;
    
    case TOKEN_IDENTIFIER:
        return parse_identifier(ctx, i, err);
        break;

    default:
        Compiler_error* err = strict_calloc(sizeof(Compiler_error), __FILE__, __LINE__);
        err->name = dynstring_do_create("SyntaxError");
        DynString* token_value = dynstring_do_get_substring(ctx->src, token->start, token->end);

        err->message = dynstring_do_create(sprintf("Expected a primary expression, but instead found "%s\"", token_value->data));
        err->line = token->line;
        err->column = token->column;
        err->filepath = ctx->filepath;
        err->raised_on_line = __LINE__;
        err->raised_in_file = dynstring_do_create(__FILE__);
        transpiler_ctx_do_push_error(ctx, err);
    }
    return NULL;
}

ASTNode* parse_expression(Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    Token* token = ctx->tokens_list[i];
    ASTNode* expr_node = parse_primary_expression(ctx, i, err);
    
    return expr_node;
}

ASTNode* parse_declaration(Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    Token* token = ctx->tokens_list[i];

    ASTNode* decl_node = strict_calloc(sizeof(ASTNode), 1, __FILE__, __LINE__);
    decl_node->kind = ASTNODE_DECLARATION;
    decl_node->start = token->start;
    decl_node->end = token->end;
    decl_node->line = token->line;
    decl_node->column = token->column;

    if (token->kind == TOKEN_VAR) {
        decl_node->props.Declaration.is_var = true;
    } else if (token->kind == TOKEN_CONST) {
        decl_node->props.Declaration.is_var = false;
    } else {
        Compiler_error* err = strict_calloc(sizeof(Compiler_error), __FILE__, __LINE__);
        err->name = dynstring_do_create("SyntaxError");
        err->message = dynstring_do_create(sprintf("Expected a declaration, but instead found "%s\"", token->value));
        err->line = token->line;
        err->column = token->column;
        err->filepath = ctx->filepath;
        err->raised_on_line = __LINE__;
        err->raised_in_file = dynstring_do_create(__FILE__);
        transpiler_ctx_do_push_error(ctx, err);
    }

    i++; // move to the next token
    if (i >= ctx->tokens->length) {
        Compiler_error* err = strict_calloc(sizeof(Compiler_error), __FILE__, __LINE__);
        err->name = dynstring_do_create("SyntaxError");
        err->message = dynstring_do_create("Expected an identifier, but instead reached the end source code");
        err->line = token->line;
        err->column = token->column;
        err->filepath = ctx->filepath;
        err->raised_on_line = __LINE__;
        err->raised_in_file = dynstring_do_create(__FILE__);
        transpiler_ctx_do_push_error(ctx, err);
    }
    token = ctx->tokens_list[i];
    if (token->kind != TOKEN_IDENTIFIER) {
        Compiler_error* err = strict_calloc(sizeof(Compiler_error), __FILE__, __LINE__);
        err->name = dynstring_do_create("SyntaxError");
        err->message = dynstring_do_create(sprintf("Expected an identifier, but instead found "%s\"", token->value));
        err->line = token->line;
        err->column = token->column;
        err->filepath = ctx->filepath;
        err->raised_on_line = __LINE__;
        err->raised_in_file = dynstring_do_create(__FILE__);
        transpiler_ctx_do_push_error(ctx, err);
    }
    decl_node->props.Declaration.identifier_node = parse_identifier(ctx, i, err);
    if (err)

}

ASTNode* parse_block(Transpiler_context* ctx, size_t* i, Compiler_error* err) {
    List* block = list_init("List<ASTNode>");

    bool in_recovery_loop = false;

    // increment the depth
    ctx->current_depth++;

    while (i < ctx->tokens->length) {
        Token* token = ctx->tokens->items[i];
        Token* next_token = i + 1 < ctx->tokens->length ? ctx->tokens->items[i + 1] : NULL;
        in_recovery_loop = false;

        if (token->kind == TOKEN_LET || token->kind == TOKEN_VAR) {
            ASTNode* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_IDENTIFIER && next_token && next_token->kind == TOKEN_ASSIGN) {
            ASTNode* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_RETURN) {
            ASTNode* ret_node = parse_return(ctx);
            if (!ret_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, ret_node);

        } else {
            if (!in_recovery_loop) {
                in_recovery_loop = true;
             
                char* msg = "Expected a statement, but instead found: ";
                size_t msg_len = strlen(msg) + strlen(token->value) + 1;
                char* err_msg = (char*)malloc(msg_len);
                if (!err_msg) {
                    perror("Failed to allocate memory for error message");
                    exit(EXIT_FAILURE);
                }
                snprintf(err_msg, msg_len, "%s%s", msg, token->value);
                perror(err_msg);
                add_error_to_list(ctx->errors, "SyntaxError", "Unexpected Token", err_msg,  ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
            }
            i++;
        }
    }
    // decrement the depth
    ctx->current_depth--;

    return block;
}

void parse_code(Transpiler_context* ctx) {

    size_t i = 0; // token index

    ASTNode* block = parse_block(ctx, &i, NULL);
    for (size_t j = 0; j < block->props.Block.statements_len; j++) {
        ASTNode* stmt = block->props.Block.statements_list[j];
        stmt->parent = ctx->ast;
        stmt->scope_owner = ctx->ast;
        stmt->depth = 0;
    }

    printf("\nNumber of Statements: %zu\n", statements->length);
    ctx->ast->props.Program.block_node = block;
}