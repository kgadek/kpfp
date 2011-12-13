%option noyywrap

%{
#include "add.tab.h"
%}

%%

[0-9]+  yylval = atoi(yytext); return NUM;
[ \t]+  /* */
.|\n    return *yytext;
