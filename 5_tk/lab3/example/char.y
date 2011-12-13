%{
#include <stdio.h>
#define YYSTYPE char
%}

%token T_CHAR

%%

input   :
        | input '\n'        { YYACCEPT; }
        | input znaki '\n'
        ;

znaki   : znaki T_CHAR      { printf("Znaleziono 1: %c\n", $2); }
        | T_CHAR            { printf("Znaleziono 2: %c\n", $1); }
        ;

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    printf("blad: %s\n", s);
}

