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

    typedef enum {CAT = 0, ALT, OTHER, STAR, PLUS, QUEST} kind;

    void print(struct node* t);
    void myFree(struct node* t);
    struct node* makeNode(char c, kind type);
    struct node* makeDouble(struct node* l, struct node* r, kind type);
    struct node* makeOne(struct node* current, struct node* child);

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
%right '*' '+' '?' NgStar NgPlus NgQuest
%left '|'

%% 
input : /* empty */
      | input line
;

line  : '\n'
      | exp '\n'    { 
                        print($<ntype>1); printf("\n");
                        $<ntype>$ = NULL;
                        //myFree($<ntype>$);
                    }
;

exp   : exp '|' cat {
                        $<ntype>$ = makeDouble($<ntype>1, $<ntype>3, ALT);
                    }
      | cat         {
                        $<ntype>$ = $<ntype>1;
                    }
;    

cat   : cat term    {
                        if ( $<ntype>1 == NULL ) {
                            $<ntype>$ = $<ntype>2;
                        } else {
                            $<ntype>$ = makeDouble($<ntype>1, $<ntype>2, CAT);
                        }
                    }
      | cat sign    {
                        $<ntype>$ = $<ntype>1;
                    }
      |             {
                        $<ntype>$ = NULL;
                    }
;

sign  : sign '*'    {
                        struct node* temp = makeNode(' ', STAR);
                        $<ntype>$ = makeOne(temp, $<ntype>1);
                    }
      |             {
                        $<ntype>$ = NULL;
                    }
;

term  : CHAR        {    
                        $<ntype>$ = makeNode($<ctype>1, OTHER);
                    } 
      |  '.'        {
                        $<ntype>$ = makeNode('.', OTHER);
                    }
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
        if ( t->right != NULL ) {
            printf(", ");
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

struct node* makeNode(char c, kind type) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    if ( type == OTHER ) {
        if ( c == '.' ) {
            temp->content = (char*) malloc(4);
            temp->content = "Dot";
        } else {
            temp->content = (char*) malloc(7);
            sprintf(temp->content, "Lit(%c)", c);
        }
    } else if ( type == STAR ) {
        temp->content = (char*) malloc(7);
        temp->content = "Star";
    }

    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

struct node* makeDouble(struct node* l, struct node* r, kind type) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    temp->content = (char*) malloc(4);
    if ( type == CAT ) {
        temp->content = "Cat";
    } else if ( type == ALT ) {
        temp->content = "Alt";
    }
    temp->left = l;
    temp->right = r;
    return temp;
}

struct node* makeOne(struct node* current, struct node* child) {
    current->left = child;
    current->right = NULL;
    return current;
}
