%{
    #include "stdio.h"
    #include "string.h"
    #include "regex.tab.h"
%}

%%

[a-zA-z]+ { yylval = strdup(yytext); return CHAR;} 
"|"     return '|';
"."     return '.';
[ \t]+

%%
