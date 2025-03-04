
Node* parse_declaration(Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];

    Node* decl_node = (Node*)malloc(sizeof(Node));
    if (!decl_node) {
        perror("Failed to allocate memory for declaration node");
        exit(EXIT_FAILURE);
    }
    decl_node->kind = NODE_DECLARATION;
    decl_node->line = token->line;
    decl_node->pos = token->pos;
    decl_node->depth = ctx->current_depth;
    decl_node->scope_owner = ctx->current_scope_owner;

    if (token->kind == TOKEN_LET) {
        decl_node->Node_Declaration.is_new = true;
        ctx->i++; // Consume 'let'
        token = ctx->tokens->items[ctx->i];
    }

    if (token->kind == TOKEN_VAR) {
        decl_node->Node_Declaration.is_var = true;
        ctx->i++; // Consume 'var'
        token = ctx->tokens->items[ctx->i];
    }

    if (token->kind != TOKEN_IDENTIFIER) {
        add_error_to_list(ctx->errors, "SyntaxError", "Invalid Declaration", "Expected an identifier in the declaration", ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
    }

    Node* ident_node = parse_identifier(ctx);
    if (!ident_node) {
        perror("Failed to parse identifier in declaration");
        return NULL;
    }
    decl_node->Node_Declaration.identifier = ident_node;
    token = ctx->tokens->items[ctx->i];

    // if there is an assignment operator, parse the expression
    // else return the declaration node

    if (token->kind != TOKEN_ASSIGN) {
        decl_node->Node_Declaration.is_assignment = false;
        return decl_node;
    }

    decl_node->Node_Declaration.is_assignment = true;
    ctx->i++; // Consume '='
    Node* expr_node = parse_expression(ctx);
    if (!expr_node) {
        perror("Failed to parse expression in declaration");
        return NULL;
    }
    decl_node->Node_Declaration.expr = expr_node;
    return decl_node;
}

Node* parse_return(Parsing_context* ctx) {
    ctx->i++;  // Consume 'return'

    if (ctx->i >= ctx->tokens->length) {
        size_t lastLine = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->line;
        size_t lastPos = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->pos;
        
        add_error_to_list(ctx->errors, "SyntaxError", "Incomplete Return Statement", "Expected an expression after the 'return' statement", ctx->filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression after the 'return' statement");
        return NULL;
    }

    Node* expr_node = parse_expression(ctx);
    if (!expr_node) {
        perror("Failed to parse expression in return statement");
        return NULL;
    }
    
    Node* ret_node = (Node*)malloc(sizeof(Node));
    if (!ret_node) {
        perror("Failed to allocate memory for return statement");
        exit(EXIT_FAILURE);
    }
    expr_node->parent = ret_node;

    ret_node->kind = NODE_RETURN;
    ret_node->line = expr_node->line;
    ret_node->pos = expr_node->pos;
    ret_node->Node_Return.expr = expr_node;

    ret_node->depth = ctx->current_depth;
    ret_node->scope_owner = ctx->current_scope_owner;

    return ret_node;
}

List* parse_block(Parsing_context* ctx) {
    List* block = list_init("List<Node>");

    bool in_recovery_loop = false;

    // increment the depth
    ctx->current_depth++;

    while (ctx->i < ctx->tokens->length) {
        Token* token = ctx->tokens->items[ctx->i];
        Token* next_token = ctx->i + 1 < ctx->tokens->length ? ctx->tokens->items[ctx->i + 1] : NULL;
        in_recovery_loop = false;

        if (token->kind == TOKEN_LET || token->kind == TOKEN_VAR) {
            Node* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_IDENTIFIER && next_token && next_token->kind == TOKEN_ASSIGN) {
            Node* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_RETURN) {
            Node* ret_node = parse_return(ctx);
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
            ctx->i++;
        }
    }
    // decrement the depth
    ctx->current_depth--;

    return block;
}

bool parse_code(List* errors, Node* program, List* tokens, char* filepath) {
    size_t current_error_count = errors->length;

    Parsing_context* ctx = (Parsing_context*)malloc(sizeof(Parsing_context));
    if (!ctx) {
        perror("Failed to allocate memory for parser context");
        exit(EXIT_FAILURE);
    }
    ctx->current_depth          = 0;
    ctx->errors                 = errors;
    ctx->tokens                 = tokens;
    ctx->filepath               = filepath;
    ctx->i                      = 0;
    ctx->current_scope_owner    = program;

    List* statements = parse_block(ctx);
    for (size_t j = 0; j < statements->length; j++) {
        Node* stmt = statements->items[j];
        stmt->parent = program;
        stmt->depth = 1;
    }

    printf("\nNumber of Statements: %zu\n", statements->length);
    program->Node_Program.block = statements;

    return errors->length == current_error_count;
}