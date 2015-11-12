%{
    #include "stdio.h"
    #include "stdlib.h"
    void yyerror (char const *);
%}

%error-verbose

%union {
    char* ctype;
}

%token CHAR
%token NUM


%% 
input : /* empty */
      | input line
;

line  : '\n' 
      | exp '\n'
;

exp : CHAR   { $<ctype>$ = $<ctype>1; printf("%s\n", $<ctype>$); }
    | '.'    { printf("Dot\n"); }

%%


int main() 
{
    return yyparse();
}


void yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
