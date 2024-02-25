#include "main.h"

char *compile_constant(struct node *ast) {
    const char *template;
    char *ret;
    if(ast->type != OP_CONSTANT) return NULL;
    template = "\tmov rax, 0x%s\n";
    ret = malloc(strlen(template)+strlen(ast->value)+1);
    sprintf(ret, template, ast->value);
    return ret;
}

char *compile_push(struct node *ast) {
    const char *template;
    char *ret;
    if(ast->type != OP_PUSH) return NULL;
    template = "\tpush rax\n";
    ret = malloc(strlen(template)+1);
    strcpy(ret, template);
    return ret;
}

char *compile_pop(struct node *ast) {
    const char *template;
    char *ret;
    if(ast->type != OP_POP) return NULL;
    template = "\tpop rbx\n";
    ret = malloc(strlen(template)+1);
    strcpy(ret, template);
    return ret;
}

char *compile_ret(struct node *ast) {
    const char *template;
    char *ret;
    if(ast->type != OP_RET) return NULL;
    template = "\tret\n";
    ret = malloc(strlen(template)+1);
    strcpy(ret, template);
    return ret;
}

#define STATEMENT_LEN 4
static char *compile_statement(struct node *ast) {
    unsigned int i;
    char *statements[STATEMENT_LEN];
    statements[0] = compile_constant(ast);
    statements[1] = compile_push(ast);
    statements[2] = compile_ret(ast);
    statements[3] = compile_pop(ast);
    for(i=0; i<STATEMENT_LEN; i++)
        if(statements[i]) return statements[i];
    return NULL;
}

char *compile_function(struct node *ast) {
    const char *template;
    unsigned int i;
    char *ret, *tmp;
    if(ast->type != OP_FUNCTION) return NULL;
    template =  "%s:\n"
                "%s\n";
    ret = calloc(1,1);
    for(i=0; i<ast->child_len; i++) {
        if(tmp = compile_statement(ast->children[i])) {
            ret = realloc(ret, strlen(ret)+strlen(tmp)+1);
            strcat(ret, tmp);
            free(tmp);
        }
    }
    tmp = ret;
    ret = malloc(strlen(template)+strlen(tmp)+strlen(ast->value)+1);
    sprintf(ret, template, ast->value, tmp);
    free(tmp);
    return ret;
}

char *compile(struct node *ast) {
    const char *template;
    char *ret, *tmp;
    unsigned int i;

    template =  "format pe64 efi\n"
                "public main\n"
                "section \".text\" executable\n"
                "%s\n";
    ret = calloc(1,1);
    for(i=0; i<ast->child_len; i++) {
        if(tmp = compile_function(ast->children[i])) {
            ret=realloc(ret, strlen(ret)+strlen(tmp)+1);
            strcat(ret, tmp);
            free(tmp);
        }
    }
    tmp = ret;
    ret = malloc(strlen(template)+strlen(tmp)+1);
    sprintf(ret, template, tmp);
    free(tmp);
    return ret;
}
