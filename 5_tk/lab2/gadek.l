%option noyywrap

%{
#include <stdio.h>
%}

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
HOST									{HOSTNAME}\.PORT

%%

{IP}									printf("\n######## IP: %s\n",yytext);
{HOSTNAME}								printf("\n######## HOSTNAME: %s\n",yytext);


%%

int main() {
	yylex();
	return 0;
}

