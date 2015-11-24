%{
#include <stdio.h>
#include <string.h>
%}

FIELD   [0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]
IPV4    {FIELD}(\.{FIELD}){3}

%%

{IPV4}(\r?\n)? {
    char *ptr = strtok(yytext, ".");
    if (ptr) {
        int num = atoi(ptr);
        if (num <= 127) {
            printf("A\n");
        } else if (num <= 191) {
            printf("B\n");
        } else if (num <= 223) {
            printf("C\n");
        } else if (num <= 239) {
            printf("D\n");
        } else if (num <= 255) {
            printf("E\n");
        } else {
            printf("Invalid\n");
        }
    }
}

.*(\r?\n)? {
    printf("Invalid\n");
}

%%

