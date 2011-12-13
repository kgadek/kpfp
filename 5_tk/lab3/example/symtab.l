%option noyywrap

%{
#include "symtab.tab.h"
%}

%%

[a-zA-Z_][0-9a-zA-Z_]*      yylval.name = (char*)strdup(yytext); return IDENT;
[0-9]+(\.[0-9]*)? |
\.[0-9]+                    yylval.value = atof(yytext); return(NUM);
[=+;]                       return(yytext[0]);
[ \t\n]+                    /*  */
.                           printf("Unrecognized character: %s\n", yytext);
