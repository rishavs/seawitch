// ast.h

#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Enum for different types of AST nodes
// typedef enum {
//     INT_NODE_TYPE,
//     DEC_NODE_TYPE,
//     BIN_OPR_NODE_TYPE,
//     PROGRAM_NODE_TYPE,
// } Node_Type;

struct AST_Node;

typedef struct {
    int value;
    char* type[16];
    struct AST_Node* parent;
    size_t pos;
    size_t line;
} Int_Node;

typedef struct {
    double value;
    char* type[16];
    struct AST_Node* parent;
    size_t pos;
    size_t line;
} Dec_Node;

// Struct for a binary operator node
typedef struct {
    char* type[16];
    struct AST_Node* parent;
    struct AST_Node* left;
    struct AST_Node* right;
} Bin_Op_Node;

typedef struct  {
    char* type[16];
    
    union {
        Int_Node int_lit;
        Dec_Node dec_lit;

        Bin_Op_Node bin_opr;
        // Add more node types as needed
    };
} Expr_Node;

typedef struct {
    char* type[16];
    struct AST_Node* parent;
    struct AST_Node* children;
} Stmt_Node;

typedef struct {
    char* type[16];
    struct AST_Node* parent;
    Stmt_Node* children;
} Block_Node;

typedef struct {
    char* type[16];
    Block_Node* child;
} Root_Node;

// Base struct for all AST nodes
typedef struct AST_Node {
    char* type[16];
    union {
        Int_Node int_lit;
        Dec_Node dec_lit;

        Bin_Op_Node bin_opr;
        // Add more node types as needed

        Expr_Node expr;
        Stmt_Node stmt;
        
        Block_Node block;
        Root_Node program;
    };
} AST_Node;

#endif // AST_H
