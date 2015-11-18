%{
    #include "stdio.h"
    #include "string.h"
    #include "regex.tab.h"
%}

%%

"(?:"   return SUB;
"("     return '(';

"*"     return '*';
"+"     return '+';
"?"     return '?';
"*?"    return NgStar;
"+?"    return NgPlus;
"??"    return NgQuest;

"|"     return '|';
"."     return '.';
"\n"    return '\n';
")"     return ')';
.       { yylval.ctype = *yytext; return CHAR; }

%%
