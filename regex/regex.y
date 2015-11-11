%{
    #include <stdio.h>
    #define YYSTYPE char*
%}

%token CHAR
%destructor { free($$); } CHAR

%% 
input  : '\n'
      | exp '\n'
;

exp : '*'   { $$ = $1;          }
    | CHAR   { printf("char= %s\n", $1);          }

%%