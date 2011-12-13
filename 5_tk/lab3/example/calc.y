%{
#include <stdio.h>

void yyerror(char *);
int yylex(void);

int sym[26];
%}

%token INTEGER VARIABLE
%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc '='

%%

program     :   program statement '\n'
            |
            ;

statement   : expression                            { printf("%d\n", $1); }
            | VARIABLE '=' expression               { sym[$1] = $3; }
            ;

expression  : INTEGER
            | VARIABLE                              { $$ = sym[$1]; }
            | expression '+' expression             { $$ = $1 + $3; }
            | expression '-' expression             { $$ = $1 - $3; }
            | '-' expression %prec UMINUS           { $$ = -$2; }
            | expression '*' expression             { $$ = $1 * $3; }
            | expression expression %prec '*'       { $$ = $1 * $2; }
            | expression '/' expression             { $$ = $1 / $3; }
            | '(' expression ')'                    { $$ = $2; }
            ;

%%

int main(void) {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    printf("blad: %s\n", s);
}


