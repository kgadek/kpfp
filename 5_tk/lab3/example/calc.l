%option noyywrap

%{
#include <stdlib.h>
#include "calc.tab.h"
void yyerror(char *);
%}

%%

[a-z]       { yylval = *yytext-'a'; return VARIABLE; }
[0-9]+      { yylval = atoi(yytext); return INTEGER; }
[-+()=/*\n] { return *yytext; }
[ \t]+      /* */
.           yyerror("Unknown character");

%%
