%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _node {
    char *str;
    struct _node* next;
} *root;
typedef struct _node node;


void addUnique(char *add, struct _node **n) {
    if( (*n)==0 ) {
        (*n)=(node*)malloc(sizeof(node));
        (*n)->str = malloc(strlen(add)+1);
        strcpy((*n)->str, add);
        (*n)->next = (node*)0;
    } else if( strcmp((*n)->str, add))
        addUnique(add,&((*n)->next));
     // wpp. znaleziono -> koniec
}
%}


SP								[ 	]
DIGIT									[0-9]
ALPHA									[a-zA-Z]
ALPHADIGIT								[a-zA-Z0-9]
WORD                                    {ALPHA}{ALPHADIGIT}*
ALPHADIGITDASH							([a-zA-Z0-9]|-)
PORT									[1-9][0-9]+
IP										[1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*
IP_WPORT								{IP}\.{PORT}
DOMAINLABEL								{ALPHADIGIT}|{ALPHADIGIT}{ALPHADIGITDASH}*{ALPHADIGIT}
TOPLABEL								{ALPHA}|{ALPHA}{ALPHADIGITDASH}*{ALPHADIGIT}
HOSTNAME								({DOMAINLABEL}\.)+{TOPLABEL}
HOST									{HOSTNAME}\.{PORT}
TIMESTAMP								({DIGIT}{2}:){2}{DIGIT}{2}\.{DIGIT}+
SHORTHOSTNAME                           {WORD}
SHORTHOST                               {WORD}\.{PORT}

IPLINE									^{TIMESTAMP}{SP}+IP{SP}+({IP}|{HOSTNAME})\.(80){1,2}{SP}+.*$
OTHERIP									^{TIMESTAMP}{SP}+IP{SP}+({IP_WPORT}|{HOST}|{SHORTHOST}){SP}+>{SP}+({IP_WPORT}|{HOST}|{SHORTHOST}):{SP}+.*$
OTHERARP								^{TIMESTAMP}{SP}+arp{SP}+{ALPHADIGITDASH}+{SP}+.*$
OTHER   								^{TIMESTAMP}{SP}+{WORD}{SP}+({IP_WPORT}|{HOST}|{SHORTHOST}){SP}+>{SP}+({IP_WPORT}|{HOST}|{SHORTHOST}):{SP}+.*$
EMPTY                                   ^\ +$
UNMATCHED                               ^.*$

%%

{IPLINE}								{
											//printf("IPLINE ### %s\n",yytext);
											char* b = strstr(yytext, "IP");
											//printf("IP ### %s\n",b+3);
											char* c = strstr(b+3," ");
											c[0]=0;
                                            //printf("%s",b+3);
                                            addUnique(b+3,&root);
										}
{OTHERIP}								{
                                            //printf("OTHER IP ### %s",yytext);
                                        }
{OTHERARP}                              {
                                            //printf("OTHER ARP ### %s",yytext);
                                        }
{OTHER}									{
                                            //printf("OTHER ### %s",yytext);
                                        }
{EMPTY}                                 {
                                            //printf("EMPTY ### %s",yytext);
                                        }
{UNMATCHED}								{
                                            //printf("UNMATCHED ### %s",yytext);
                                            fprintf(stderr,"Błędny format: >%s<\n",yytext);
                                            exit(1);
                                        }

%%


int main() {
    root = (node*)0;
	yylex();
    while(root != 0) {
        printf("%s\n", root->str);
        root = root->next;
    }
	return 0;
}

