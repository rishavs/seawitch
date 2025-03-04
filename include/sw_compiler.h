#ifndef SEAWITCH_COMPILER_H
#define SEAWITCH_COMPILER_H

#include <stdbool.h>
#include <stdio.h>

#include "sw_core.h"

#include "seawitch.h"

// Define the macro for the enum and string array
#define TOKEN_KINDS  \
    X(TOKEN_ILLEGAL) \
    X(TOKEN_ASSIGN)  \
    X(TOKEN_PLUS)    \
    X(TOKEN_MINUS)   \
    X(TOKEN_IDENT)   \
    X(TOKEN_INTEGER) \
    X(TOKEN_DECIMAL) \
    X(TOKEN_RETURN)  \
    X(TOKEN_LPAREN)  \
    X(TOKEN_RPAREN)  \
    X(TOKEN_CONST)   \
    X(TOKEN_VAR)     \
    X(TOKEN_COUNT_X)

// Generate the enum using the macro
typedef enum
{
#define X(token) token,
    TOKEN_KINDS
#undef X
} Token_kind_enum;

// Generate the string array using the macro
static const char *Token_kind_strings[] = {
#define X(token) #token,
    TOKEN_KINDS
#undef X
};

typedef struct
{
    Token_kind_enum kind;

    size_t start;
    size_t end;

    size_t line;
    size_t column;
} Token;

// Define the macro for the enum and string array
#define ASTNODE_KINDS  \
    X(ASTNODE_PROGRAM) \
    X(ASTNODE_DECLR)   \
    X(ASTNODE_IDENT)   \
    X(ASTNODE_INTEGER) \
    X(ASTNODE_COUNT_X)

// Generate the enum using the macro
typedef enum
{
#define X(astnode) astnode,
    ASTNODE_KINDS
#undef X
} ASTNode_kind_enum;

// Generate the string array using the macro
static const char *ASTNode_kind_strings[] = {
#define X(astnode) #astnode,
    ASTNODE_KINDS
#undef X
};

typedef struct ASTNode ASTNode; // Forward declaration
struct ASTNode
{
    ASTNode_kind_enum kind;

    size_t start;
    size_t end;
    size_t line;
    size_t column;
    size_t depth;         // distance from root
    ASTNode *parent;      // immediate parent
    ASTNode *scope_owner; // nearest scope owner

    // Node specific data
    union {
        struct {
            bool is_var;
            bool is_new;
            bool is_assignment;
            ASTNode *Identifier;
            ASTNode *Expression;
        } Declaration;
        struct {
            ASTNode **list;
            size_t len;
            size_t capacity;
        } Statements;
        struct {
            char *filepath;
            ASTNode *Statements;
        } Program;
    } props;
};

static const char *default_c_code = "\
int main() {\n\
    return 0;\n\
}";

typedef struct
{
    Compiler_error **error_refs_list;
    size_t error_len;
    size_t error_capacity;

    // Reader
    char *filepath;
    size_t filepath_len;
    char *src;
    size_t src_len;
    double reading_duration;

    // Lexer
    Token *tokens_list;
    size_t token_len;
    size_t token_capacity;
    double lexing_duration;

    // Parser
    ASTNode *ast;
    double parsing_duration;

} Transpiler_context;
void transpile_code(Transpiler_context *ctx);
Transpiler_context *transpiler_ctx_do_init(char *filepath, char *src);
void transpiler_ctx_do_push_error(Transpiler_context *ctx, Compiler_error *err);
void transpiler_ctx_do_push_token(Transpiler_context *ctx, Token token);

void lex_code(Transpiler_context *ctx);

#endif