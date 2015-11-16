%{
    #include "stdio.h"
    #include "stdlib.h"
    #include "string.h"
    void yyerror (char const *);
    
    struct node {
        char* content;
        struct node* left;
        struct node* right;
    };

    void print(struct node* t);
    void myFree(struct node* t);
    struct node* makeChar(char c);
    struct node* cat(struct node* l, struct node* r);
    struct node* makeSingle(char* cs, struct node* child);

    int parenCount = 1;
%}

%error-verbose

%union {
    struct node* ntype;
    char* cstype;
    char  ctype;
    int   itype;
}

%token CHAR SUB
%right NgStar NgPlus NgQuest
%left '|'

%% 
input : /* empty */
      | input line
;

line  : '\n'
      | exp '\n'    { print($<ntype>1); printf("\n"); //myFree($<ntype>$);
                    }
;

exp : exp CHAR  {    
                    struct node* t = makeChar($<ctype>2);
                    if ( $<ntype>1 == NULL ) {
                        $<ntype>$ = t;
                    } else {
                        $<ntype>$ = cat($<ntype>1, t);
                    }
                }  

    | exp '.'   {
                    struct node* t = makeChar('.');
                    if ( $<ntype>1 == NULL ) {
                        $<ntype>$ = t;
                    } else {
                        $<ntype>$ = cat($<ntype>1, t);
                    }
                }

    |          {   $<ntype>$ = NULL; }
;    

%%


int main() {
    return yyparse();
}

void yyerror (char const *s) {
    fprintf (stderr, "%s\n", s);
}


void print(struct node* t) {
    printf("%s", t->content);
    if ( t->left != NULL | t->right != NULL ) {
        printf("(");
        if ( t->left != NULL ) {
            print(t->left);
        }
        printf(", ");
        if ( t->right != NULL ) {
            print(t->right);
        }
        printf(")");
    }
}

void myFree(struct node* t) {
    if ( t->left != NULL ) {
        myFree(t->left);
    }
    if ( t->right != NULL ) {
        myFree(t->right);
    }
    free(t->content);
    free(t);
    t = NULL;
}

struct node* makeChar(char c) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    if ( c == '.' ) {
        temp->content = (char*) malloc(4);
        temp->content = "Dot";
    } else {
        temp->content = (char*) malloc(7);
        sprintf(temp->content, "Lit(%c)", c);
    }
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

struct node* cat(struct node* l, struct node* r) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    temp->content = (char*) malloc(4);
    temp->content = "Cat";
    temp->left = l;
    temp->right = r;
    return temp;
}

struct node* makeSingle(char* cs, struct node* child) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    temp->content = (char*) malloc(strlen(cs) + 1);
    strcpy(temp->content, cs);
    temp->left = child;
    temp->right = NULL;
    return temp;
}
