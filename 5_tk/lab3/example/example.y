%token LETTER

%{
#include <stdio.h>
%}

%%

input: LETTER   { printf("rozpoznano -%c\n", $1); };

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    printf("blad: %s\n", s);
}

