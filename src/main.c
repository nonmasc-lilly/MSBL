#include "main.h"
#include "License.h"

void help(int errcode);

int main(int argc, char **argv) {
    FILE *fp;
    char *content, *compiled;
    const char *ifile = NULL;
    const char *ofile = NULL;
    unsigned int i, fsz, lsz;
    struct token *lexed;
    struct node *parsed;
    BOOL debug = FALSE;
    printf(LICENSE_NOTIFICATION);

    if(argc < 2) {
        printf("Error:: no arguments\n");
        help(1);
    }

    for(i=1; i<argc; i++) {
        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
            case 'h': help(0); break;
            case 'o':
                ofile = argv[i+1];
                i++;
                break;
            case 'd':
                debug = TRUE;
                break;
            }
        } else {
            if(ifile != NULL) {
                 printf("Error:: multiple input files\n");
                 exit(1);
            }
            ifile = argv[i]; 
        }
    }

    if(!ifile) {
        printf("Error:: no input files\n");
        exit(1);
    }
    if(!ofile) ofile = "a.out";

    fp = fopen(ifile, "r");
    content = malloc(fsz = (fseek(fp, 0L, SEEK_END), ftell(fp)));
    fseek(fp, 0L, SEEK_SET);
    fread(content, 1, fsz, fp);
    fclose(fp);

    lex(content, &lexed, &lsz);
    if(debug) replexed(lexed, lsz);

    parsed = parse(lexed, lsz);
    if(debug) repast(parsed, 0);

    compiled = compile(parsed);
    printf("%s", compiled);

    free(content);
    return 0;
}

void help(int errcode) {
    printf("MSBL is a stack based programming language\n"
           "designed for use on small UEFI operating\n"
           "systems. This is Lilly Taus' MSBL compiler.\n");
    printf("This compiler has the following options:\n"
           "\t-h : shows this menu\n"
           "\t-o : sets the name of the output file\n"
           "\t     (defualt : a.out)\n"
           "\t-d : shows debug messages\n");
    exit(errcode);
}
