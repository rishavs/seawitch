
ASTNode* parse_expression(Parsing_context* ctx);

ASTNode* parse_integer (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    ASTNode* int_ASTNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (!int_ASTNode) {
        perror("Failed to allocate memory for integer ASTNode");
        exit(EXIT_FAILURE);
    }
    int_ASTNode->kind = ASTNode_INTEGER;
    int_ASTNode->ASTNode_Integer.value = token->value;
    int_ASTNode->line = token->line;
    int_ASTNode->pos = token->pos;
    int_ASTNode->depth = ctx->current_depth;
    int_ASTNode->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return int_ASTNode;
};

ASTNode* parse_decimal (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    ASTNode* dec_ASTNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (!dec_ASTNode) {
        perror("Failed to allocate memory for integer ASTNode");
        exit(EXIT_FAILURE);
    }
    dec_ASTNode->kind = ASTNode_DECIMAL;
    dec_ASTNode->ASTNode_Decimal.value = token->value;
    dec_ASTNode->line = token->line;
    dec_ASTNode->pos = token->pos;
    dec_ASTNode->depth = ctx->current_depth;
    dec_ASTNode->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return dec_ASTNode;
};

ASTNode* parse_identifier (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    ASTNode* ident_ASTNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (!ident_ASTNode) {
        perror("Failed to allocate memory for identifier ASTNode");
        exit(EXIT_FAILURE);
    }
    ident_ASTNode->kind = ASTNode_IDENTIFIER;
    ident_ASTNode->ASTNode_Identifier.value = token->value;
    ident_ASTNode->line = token->line;
    ident_ASTNode->pos = token->pos;
    ident_ASTNode->depth = ctx->current_depth;
    ident_ASTNode->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return ident_ASTNode;
};

ASTNode* parse_primary_expression(Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    if (token->kind == TOKEN_INTEGER) {
        return parse_integer(ctx);
    } else if (token->kind == TOKEN_DECIMAL) {
        return parse_decimal(ctx);
    } else if (token->kind == TOKEN_LPAREN) {
        return parse_grouped_expression(ctx);
    } else if (token->kind == TOKEN_IDENTIFIER) {
        return parse_identifier(ctx);
    } else {
        add_error_to_list(ctx->errors, "SyntaxError", "Invalid Expression", "Expected an expression", ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a primary expression in the primary expression function");
        return NULL;
    }
}


ASTNode* parse_expression(Parsing_context* ctx) {
    if (ctx->i >= ctx->tokens->length) {
        size_t lastLine = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->line;
        size_t lastPos = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->pos;
        add_error_to_list(ctx->errors, "SyntaxError", "Incomplete Expression", "Expected an expression", ctx->filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression");
        return NULL;
    }
    ASTNode* binary_expr = parse_binary_expression(ctx);

    return binary_expr;
}
