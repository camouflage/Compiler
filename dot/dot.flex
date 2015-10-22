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

COMMENT   \/\/(.)*|\/\*(.|\n)*\*\/|#(.)*

STRING   \"(.)*\"

NUM      (-)?([0-9])*(\.)?([0-9])*

ID       [_a-zA-Z]([_a-zA-Z0-9])*

WHITE    [\ \t\n]

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

{STRING}|{NUM}|{ID} {
    printf("%s\n", yytext);
}

{WHITE} {}

%%
