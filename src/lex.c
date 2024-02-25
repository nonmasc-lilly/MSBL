#include "main.h"

BOOL is_ws(char c) {
    switch(c) {
     case ' ':
    case '\t':
    case '\r':
    case '\n': return TRUE;
      default: return FALSE;
    }
}

OP_TYPE op_which(char c) {
    switch(c) {
    case '$': return OP_PUSH;
    case '{': return OP_COP;
    case '}': return OP_CCL;
    case '.': return OP_POP;
    default:  return OP_NULL;
    }
}

OP_TYPE tok_which(char *s) {
    char *endptr;
    if(*s == 0) return OP_NULL;
    
    IFNCMP(s, "ret") return OP_RET;

    strtol(s, &endptr, 16);
    if(!(*endptr)) return OP_CONSTANT;
    return OP_IDEN;
}

BOOL tok_has_value(OP_TYPE type) {
    switch(type) {
    case OP_IDEN:
    case OP_CONSTANT: return TRUE;
    default: return FALSE;
    }
}

ERR_CODE lex(char *s, struct token **ret, unsigned int *rsz) {
    unsigned int i, bsz;
    char *buf;

    *ret = malloc(sizeof(struct token));
    *rsz = 0;
     buf = calloc(1,1);
     bsz = 1;
    for(i=0; s[i]; i++) {
        if(op_which(s[i])) {
            if(tok_which(buf)) {
                INC_SIZE(*ret, *rsz);
                (*ret)[(*rsz)-1].type = tok_which(buf);
                if(tok_has_value(tok_which(buf))) {
                    (*ret)[(*rsz)-1].value = malloc(strlen(buf));
                    strcpy((*ret)[(*rsz)-1].value, buf);
                }
                free(buf);
                buf = calloc(1,1);
                bsz = 1;
            }
            INC_SIZE(*ret, *rsz);
            (*ret)[(*rsz)-1].type = op_which(s[i]);
            continue;
        }
        if(is_ws(s[i])) {
            if(tok_which(buf)) {
                INC_SIZE(*ret, *rsz);
                (*ret)[(*rsz)-1].type = tok_which(buf);
                if(tok_has_value(tok_which(buf))) {
                    (*ret)[(*rsz)-1].value = malloc(strlen(buf));
                    strcpy((*ret)[(*rsz)-1].value, buf);
                }
                free(buf);
                buf = calloc(1, 1);
                bsz = 1;
            }
            continue;
        }
        INC_SIZE(buf, bsz);
        buf[bsz-2] = s[i];
        buf[bsz-1] = 0;
    }
    free(buf);
    return 0;
}

const char *reptok(OP_TYPE type) {
    switch(type) {
    case OP_CONSTANT: return "constant";
    case OP_PUSH:     return "push";
    case OP_IDEN:     return "iden";
    case OP_COP:      return "{";
    case OP_CCL:      return "}";
    case OP_RET:      return "ret";
    default:          return "null";
    }
}

void replexed(struct token *lexed, unsigned int sz) {
    unsigned int i;
    printf("replex {\n");
    for(i=0;i<sz;i++) {
        printf("\t%d: %s\n", i, reptok(lexed[i].type));
    }
    printf("}\n");
}
