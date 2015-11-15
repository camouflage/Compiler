%{
    #include "stdio.h"
    #include "stdlib.h"
    #include "string.h"
    void yyerror (char const *);
    struct node* makeChar(char c);
    
    struct node {
        char* content;
        struct node* left;
        struct node* right;
    };

%}

%error-verbose

%union {
    struct node* ntype;
    char* cstype;
    char  ctype;
    int   itype;
}

%token CHAR SUB
%left NgStar NgPlus NgQuest
%left '|'

%% 
input : /* empty */
      | input line
;

line  : '\n'
      | exp '\n'
;

exp : term exp
    | 
;

term : CHAR {        
                    $<ntype>$ = makeChar($<ctype>1);
                    printf("%s\n", $<ntype>$->content);

            }
%%


int main() {
    return yyparse();
}


void yyerror (char const *s) {
    fprintf (stderr, "%s\n", s);
}


struct node* makeChar(char c) {
    struct node* temp = (struct node*) malloc(sizeof(struct node));
    temp->content = (char*) malloc(6);
    sprintf(temp->content, "Lit:%c", c);
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}
