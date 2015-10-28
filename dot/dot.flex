%{
    #include "stdio.h"
%}

STRICT   [sS][tT][rT][iI][cC][tT]
GRAPH    [gG][rR][aA][pP][hH]
DIGRAPH  [dD][iI][gG][rR][aA][pP][hH]
SUBGRAPH [sS][uU][bB][gG][rR][aA][pP][hH]
NODE     [nN][oO][dD][eE]
EDGE     [eE][dD][gG][eE]

DELIMIT  [;,{}\[\]]

CHAR     --|->|=

COMMENT   \/\/(.)*|#(.)*

NUM      (-)?([0-9])+(\.)?([0-9])*|(-)?([0-9])*(\.)?([0-9])+

ID       [_a-zA-Z]([_a-zA-Z0-9])*

WHITE    [\ \t\n]

%x MultiLineComment
%x STRING

%%

{STRICT}|{GRAPH}|{DIGRAPH}|{SUBGRAPH}|{NODE}|{EDGE} {
    printf("%s\n", yytext);
}

{DELIMIT} {
    printf("%s\n", yytext);
}

{CHAR} {
    printf("%s\n", yytext);
}

{COMMENT} {}

{NUM}|{ID} {
    printf("%s\n", yytext);
}

{WHITE} {}

"/*"                          BEGIN(MultiLineComment);
<MultiLineComment>([^*\n])*
<MultiLineComment>\*([^*/\n])*
<MultiLineComment>\n             
<MultiLineComment>"*/"        BEGIN(INITIAL);
    
\"                            {
                                printf("\"");
                                BEGIN(STRING);
                              }
<STRING>\\\"                  { 
                                printf("%s", yytext);
                              }
<STRING>[^"\n]                {
                                printf("%s", yytext);
                              }
<STRING>\"                    {  
                                printf("%s\n", yytext);
                                BEGIN(INITIAL);
                              }
%%
