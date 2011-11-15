%option noyywrap

%{
#include <stdio.h>
%}

WHITESPACE								[ 	]
DIGIT									[0-9]
ALPHA									[a-zA-Z]
ALPHADIGIT								[a-zA-Z0-9]
ALPHADIGITDASH							([a-zA-Z0-9]|-)
PORT									[1-9][0-9]+
IP										[1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*
IP_WPORT								{IP}\.{PORT}
DOMAINLABEL								{ALPHADIGIT}|{ALPHADIGIT}{ALPHADIGITDASH}*{ALPHADIGIT}
TOPLABEL								{ALPHA}|{ALPHA}{ALPHADIGITDASH}*{ALPHADIGIT}
HOSTNAME								({DOMAINLABEL}\.)+{TOPLABEL}
HOST									{HOSTNAME}\.{PORT}
TIMESTAMP								({DIGIT}{2}:){2}{DIGIT}{2}\.{DIGIT}+

IPLINE									^{TIMESTAMP}{WHITESPACE}+IP{WHITESPACE}+({IP}|{HOSTNAME})\.(80){1,2}{WHITESPACE}+.*$
OTHER									^.*$

%%

{IPLINE}								printf("IPLINE ### %s",yytext);
{OTHER}									printf("OTHER ### %s",yytext);

%%

int main() {
	yylex();
	return 0;
}

