%option noyywrap

%{
#include "example.tab.h"
%}

%%

-[abc]      { yylval = yytext[1]; return LETTER; }
.|\n        ;
