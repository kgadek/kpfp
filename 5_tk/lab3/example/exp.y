%{
#include <cstdio>

int yylex();
int yyerror(const char *s);
%}


%token NUM

%left '-'

%%

input 	:
	  	|	input line
		;

line	:	'\n'
	 	| 	exp '\n'		{ printf("%d\n", $1); }
		;

exp		:	NUM
	 	|	exp '-' exp		{ $$ = $1 - $3; }
		|	'(' exp ')'		{ $$ = $2; }
		;


%%

int main() {
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    printf("blad: %s\n", s);
}

