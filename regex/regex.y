%{
    #include "stdio.h"
    #include "stdlib.h"
    #include "string.h"
    void yyerror (char const *);
    
    struct node {
        int isParen;
        char* content;
        struct node* left;
        struct node* right;
    };

    typedef enum {CAT = 0, ALT, OTHER, STAR, PLUS, QUEST, NGSTAR, NGPLUS, NGQUEST, PAREN} kind;

    void print(struct node* t);
    void myFree(struct node* t);
    struct node* makeNode(char c, kind type, struct node* l, struct node* right);

    int parenCount = 1;
%}


%error-verbose

%union {
    struct node* ntype;
    char  ctype;
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
                        parenCount = 1;
                        myFree($<ntype>1);
                    }
;

exp   : exp '|' cat {
                        $<ntype>$ = makeNode(' ', ALT, $<ntype>1, $<ntype>3);
                    }
      | cat         {
                        $<ntype>$ = $<ntype>1;
                    }
;    

cat   : cat sign   {
                        $<ntype>$ = makeNode(' ', CAT, $<ntype>1, $<ntype>2);
                    }
      | sign        {
                        $<ntype>$ = $<ntype>1;
                    }
;

sign  : sign '*'    {
                        $<ntype>$ = makeNode(' ', STAR, $<ntype>1, NULL);
                    }
      | sign '+'    {
                        $<ntype>$ = makeNode(' ', PLUS, $<ntype>1, NULL);
                    }
      | sign '?'    {
                        $<ntype>$ = makeNode(' ', QUEST, $<ntype>1, NULL);
                    }
      | sign NgStar {
                        $<ntype>$ = makeNode(' ', NGSTAR, $<ntype>1, NULL);
                    }
      | sign NgPlus {
                        $<ntype>$ = makeNode(' ', NGPLUS, $<ntype>1, NULL);
                    }
      | sign NgQuest{
                        $<ntype>$ = makeNode(' ', NGQUEST, $<ntype>1, NULL);
                    }
      | paren       {
                        $<ntype>$ = $<ntype>1;
                    }
;

paren : '(' exp ')' {
                        $<ntype>$ = makeNode(' ', PAREN, $<ntype>2, NULL);
                    }
      | SUB exp ')' {
                        $<ntype>$ = $<ntype>2;
                    }
      | term        {
                        $<ntype>$ = $<ntype>1;
                    }
;

term  : CHAR        {    
                        $<ntype>$ = makeNode($<ctype>1, OTHER, NULL, NULL);
                    } 
      |  '.'        {
                        $<ntype>$ = makeNode('.', OTHER, NULL, NULL);
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
    if ( !t->isParen ) {
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
    } else {
        printf("(%d, ", parenCount++);
        print(t->left);
        printf(")");
    }
}

void myFree(struct node* t) {
    if ( t->left != NULL ) {
        myFree(t->left);
        t->left = NULL;
    }
    if ( t->right != NULL ) {
        myFree(t->right);
        t->right = NULL;
    }
    
    if ( t->content != NULL ) {
        free(t->content);
        t->content = NULL;
    }
    
    if ( t != NULL ) {
        free(t);
        t = NULL;
    }
}

struct node* makeNode(char c, kind type, struct node* l, struct node* r) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    temp->isParen = 0;
    temp->left = NULL;
    temp->right = NULL;

    if ( type == OTHER ) {
        if ( c == '.' ) {
            temp->content = (char*) malloc(4);
            sprintf(temp->content, "Dot");
        } else {
            temp->content = (char*) malloc(7);
            sprintf(temp->content, "Lit(%c)", c);
        }
    } else {
        temp->left = l;
        if ( type == CAT ) {
            temp->content = (char*) malloc(4);
            sprintf(temp->content, "Cat");
            temp->right = r;
        } else if ( type == ALT ) {
            temp->content = (char*) malloc(4);
            sprintf(temp->content, "Alt");
            temp->right = r;
        } else if ( type == STAR ) {
            temp->content = (char*) malloc(5);
            sprintf(temp->content, "Star");
        } else if ( type == PLUS ) {
            temp->content = (char*) malloc(5);
            sprintf(temp->content, "Plus");
        } else if ( type == QUEST ) {
            temp->content = (char*) malloc(6);
            sprintf(temp->content, "Quest");
        } else if ( type == NGSTAR ) {
            temp->content = (char*) malloc(7);
            sprintf(temp->content, "NgStar");
        } else if ( type == NGPLUS ) {
            temp->content = (char*) malloc(7);
            sprintf(temp->content, "NgPlus");
        } else if ( type == NGQUEST ) {
            temp->content = (char*) malloc(8);
            sprintf(temp->content, "NgQuest");
        } else if ( type == PAREN ) {
            temp->content = (char*) malloc(6);
            sprintf(temp->content, "Paren");
            temp->isParen = 1;
        }
    }

    return temp;
}
