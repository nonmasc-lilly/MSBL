#include "main.h"

struct node *ast_new() {
    struct node *ret;
    ret = malloc(sizeof(struct node));
    ret->children = malloc(1);
    ret->child_len=0;
    return ret;
}

void ast_del(struct node *ast) {
    unsigned int i;
    for(i=0; i<ast->child_len; i++) {
        ast_del(ast->children[i]);
    }
    free(ast->children);
    free(ast);
}

void ast_child_add(struct node *ast, struct node *child) {
    ast->children = realloc(ast->children,
        (++ast->child_len)*sizeof(struct node));
    ast->children[ast->child_len-1]=child;
}

struct node *parse_push(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    struct node *ret;
    if(lexed[offset].type != OP_PUSH) return NULL;
    ret = ast_new();
    ret->type = OP_PUSH;
    (*inc)++;
    return ret;
}

struct node *parse_pop(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    struct node *ret;
    if(lexed[offset].type != OP_POP) return NULL;
    ret = ast_new();
    ret->type = OP_POP;
    (*inc)++;
    return ret;
}

struct node *parse_ret(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    struct node *ret;
    if(lexed[offset].type != OP_RET) return NULL;
    ret = ast_new();
    ret->type = OP_RET;
    (*inc)++;
    return ret;
}

struct node *parse_constant(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    struct node *ret;
    if(lexed[offset].type != OP_CONSTANT) return NULL;
    ret = ast_new();
    ret->type = OP_CONSTANT;
    SETSTR(ret->value, lexed[offset].value);
    (*inc)++;
    return ret;
}

#define STATEMENT_LEN 4
struct node *parse_statement(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    unsigned int i;
    struct node *statements[STATEMENT_LEN];
    statements[0] = parse_constant(lexed, offset, inc);
    statements[1] = parse_push(lexed, offset, inc);
    statements[2] = parse_ret(lexed, offset, inc);
    statements[3] = parse_pop(lexed, offset, inc);
    for(i=0; i<STATEMENT_LEN; i++)
        if(statements[i]) return statements[i];
    return NULL;
}

struct node *parse_function(struct token *lexed, unsigned int offset,
        unsigned int *inc) {
    struct node *ret, *tmp;
    if(lexed[offset].type != OP_IDEN || lexed[offset+1].type != OP_COP)
        return NULL;
    ret = ast_new();
    ret->type = OP_FUNCTION;
    SETSTR(ret->value, lexed[offset].value);
    offset = 0;
    *inc+=2;
    while(1) {
        if(tmp = parse_statement(lexed, offset+*inc, inc)) {
            ast_child_add(ret, tmp);
        } else ASSERT(0, "expected statement\n", offset+*inc);
        if(lexed[offset+*inc].type == OP_CCL) break;
    }
    *inc += 1;
    return ret;
}

struct node *parse(struct token *lexed, unsigned int lsz) {
    struct node *ret, *tmp;
    unsigned int offset, inc;

    ret = ast_new();
    ret->type = OP_NULL;
    ret->value = NULL;
    offset = 0;

    while(1){
        inc = 0;
        if(tmp = parse_function(lexed, offset, &inc)) {
            offset+=inc;
            ast_child_add(ret, tmp);
        } else ASSERT(0, "expected function\n", offset);
        if(offset >= lsz) break;
    }
    return ret;
}

void repast(struct node *ast, unsigned int level) {
    unsigned int i;
    for(i=0; i<level; i++) {
        printf("|");
    }
    
    if(level != 0) printf("> %s\n", reptok(ast->type));
    else printf("> root\n");
    for(i=0; i<ast->child_len; i++) {
        repast(ast->children[i], level+1);
    }
}
