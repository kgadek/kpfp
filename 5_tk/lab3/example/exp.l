%option noyywrap

%{
#include "exp.tab.h"
%}

%%

[0-9]+		yylval = atoi(yytext); return NUM;
[ \t]+		/* eat spaces */
.|\n		return *yytext;

