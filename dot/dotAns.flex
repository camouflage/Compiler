%{
#include <stdio.h>
%}

%s COMMENT

STRICT          [Ss][Tt][Rr][Ii][Cc][Tt]
GRAPH           [Gg][Rr][Aa][Pp][Hh]
DIGRAPH         [Dd][Ii][Gg][Rr][Aa][Pp][Hh]
NODE            [Nn][Oo][Dd][Ee]
EDGE            [Ee][Dd][Gg][Ee]
SUBGRAPH        [Ss][Uu][Bb][Gg][Rr][Aa][Pp][Hh]

DIGIT           [0-9]
NUMBER          -?(\.{DIGIT}+|{DIGIT}+(\.{DIGIT}*)?)

STRING          \"([^\r\n\"]|\\\")*\"

LETTER          [a-zA-Z_]
ID              {LETTER}({LETTER}|{DIGIT})*

LINE_COMMENT    \/\/.*
PREPROC         #.*

WS              [ \t\n\r]+

%%

<INITIAL>{
    {STRICT}   printf("%s\n", yytext);
    {GRAPH}    printf("%s\n", yytext);
    {DIGRAPH}  printf("%s\n", yytext);
    {SUBGRAPH} printf("%s\n", yytext);
    {NODE}     printf("%s\n", yytext);
    {EDGE}     printf("%s\n", yytext);
    {NUMBER}   printf("%s\n", yytext);
    {ID}       printf("%s\n", yytext);
    {STRING}   printf("%s\n", yytext);
    ";"        printf("%s\n", yytext);
    ","        printf("%s\n", yytext);
    "{"        printf("%s\n", yytext);
    "}"        printf("%s\n", yytext);
    "["        printf("%s\n", yytext);
    "]"        printf("%s\n", yytext);
    "--"       printf("%s\n", yytext);
    "->"       printf("%s\n", yytext);
    "="        printf("%s\n", yytext);
    {PREPROC}       ;
    {LINE_COMMENT}  ;
    "/*"            BEGIN COMMENT;
    {WS}            ;
    .               ;
}

<COMMENT>{
    "*/"    BEGIN INITIAL;
    .|\r?\n ;
}

%%

