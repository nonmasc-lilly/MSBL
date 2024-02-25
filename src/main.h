#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAIN_H
#define MAIN_H

#define IFNCMP(a,b) if(!strcmp(a,b))
#define ELNCMP(a,b) else IFNCMP(a,b)

#define INC_SIZE(arr, arrsz)\
        arr = realloc(arr, ((arrsz)+1)*sizeof(*arr));\
        (arrsz)+=1

#define ASSERT(cond, str, off)\
        do {\
            if(!cond) {\
                printf("%s\n\tat %d\n", str, off);\
                exit(1);\
            }\
        } while(0)

#define FALSE 0
#define TRUE 1

#define SETSTR(dest, src)\
        dest = malloc(strlen(src)+1);\
        strcpy(dest, src)

typedef unsigned int BOOL;
typedef unsigned int ERR_CODE;

typedef enum {
    OP_NULL,
    OP_CONSTANT,
    OP_PUSH,
    OP_POP,
    OP_COP,
    OP_CCL,
    OP_RET,
    OP_FUNCTION,
    OP_IDEN
} OP_TYPE;

struct token {
    OP_TYPE type;
    char *value;
};

struct node {
    OP_TYPE type;
    char *value;
    unsigned int child_len;
    struct node **children;
};

BOOL is_ws(char c);
OP_TYPE op_which(char c);
OP_TYPE tok_which(char *s);
BOOL tok_has_value(OP_TYPE type);
ERR_CODE lex(char *s, struct token **ret, unsigned int *rsz);
const char *reptok(OP_TYPE type);
void replexed(struct token *lexed, unsigned int sz);

struct node *ast_new();
void ast_del(struct node *ast);
void ast_child_add(struct node *ast, struct node *child);
struct node *parse(struct token *lexed, unsigned int lsz);
void repast(struct node *ast, unsigned int level);

char *compile(struct node *ast);

#endif
