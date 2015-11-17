%{
    #include "stdio.h"
    #include "string.h"
    #include "regex.tab.h"
%}

%%

"*?"    return NgStar;
"+?"    return NgPlus;
"??"    return NgQuest;

"(?:"   return SUB;
"("     return '(';
"|"     return '|';
"."     return '.';
"*"     return '*';
"+"     return '+';
"?"     return '?';
"\n"    return '\n';
")"     return ')';
.       { yylval.ctype = *yytext; return CHAR; }

%%
