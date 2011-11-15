%{
#include <stdio.h>
%}

%%

lex										printf("flex");

%%

int main() {
	yylex();
	return 0;
}

