%{
    #include "stdio.h"
    #include "string.h"
    #include "stdlib.h"
    #include "regex.tab.h"
%}

%%

[a-zA-z]+ { yylval.ctype = strdup(yytext); return CHAR; }

"|"     return '|';
"."     return '.';
"\n"    return '\n';
[ \t]+  ;
.       ;

%%
