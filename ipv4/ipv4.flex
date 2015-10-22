%{
    #include "stdio.h"
%}

Z2TFF     [0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]
Z2OTS     [0-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7]
OTE2ONO   12[8-9]|1[3-8][0-9]|19[0-1]
ONT2TTT   19[2-9]|2[0-1][0-9]|22[0-3]
TTF2TTN   22[4-9]|23[0-9]
TFT2TFF   24[0-9]|25[0-5]
A          ^{Z2OTS}\.{Z2TFF}\.{Z2TFF}\.{Z2TFF}(\n)?
B          ^{OTE2ONO}\.{Z2TFF}\.{Z2TFF}\.{Z2TFF}(\n)?
C          ^{ONT2TTT}\.{Z2TFF}\.{Z2TFF}\.{Z2TFF}(\n)?
D          ^{TTF2TTN}\.{Z2TFF}\.{Z2TFF}\.{Z2TFF}(\n)?
E          ^{TFT2TFF}\.{Z2TFF}\.{Z2TFF}\.{Z2TFF}(\n)?
OTHER      ^(.)*(\n)?

%%

{A} {
    printf("A\n");
}

{B} {
    printf("B\n");
}

{C} {
    printf("C\n");
}

{D} {
    printf("D\n");
}

{E} {
    printf("E\n");
}

{OTHER} {
    printf("Invalid\n");
}

%%
