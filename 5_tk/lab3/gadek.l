%option noyywrap
%option yylineno

%{
#include <cstdio>
#include "gadek.tab.h"

void yyerror(const char *);
%}

ADDR					"http://"[a-zA-Z0-9/\. #]+
BAD_ADDR				[^"http://"][a-zA-Z0-9/\. #]+

%s TAGNAME START LPODMIOT HPODMIOT END ORZECZENIEB TAG_STA TAG_STO BLOK_GLOWNY DOMENA_GLOWNA ATRYBUTY

%%

<INITIAL>"<rdf:RDF"[ \t\n]"xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""[ \t\n]+"xmlns:cd=\""	BEGIN(HPODMIOT);

<HPODMIOT>{ADDR}"\">"					BEGIN(BLOK_GLOWNY); {
											yytext[yyleng-2]=0;
											yylval.webpage = yytext;
											return STRONA_WEB;
										}

<HPODMIOT>{BAD_ADDR}					{
											fprintf(stderr, "Bledny adres, linia %d\n", yylineno + ~-0 );
											yyerror("Blad!");
										}

<BLOK_GLOWNY>"<rdf:Description"			BEGIN(DOMENA_GLOWNA);

<DOMENA_GLOWNA>"rdf:about=\""			BEGIN(LPODMIOT);

<LPODMIOT>{ADDR}"\">"					BEGIN(TAG_STA); {
											yytext[yyleng-2] = 0;
											yylval.subject = yytext;
											return PODMIOT;
										}

<LPODMIOT>{BAD_ADDR}					BEGIN(BLOK_GLOWNY); {
											fprintf(stderr, "Bledny adres, linia %d\n", yylineno + ~-0);
											yyerror("Blad");
										}

<TAG_STA>"<cd:"							BEGIN(TAGNAME);

<TAGNAME>[a-zA-Z]+						BEGIN(ORZECZENIEB); {
											yylval.predicate = yytext;
											return ORZECZENIE;
										}

<ORZECZENIEB>{ADDR}/"\" />"				BEGIN(TAG_STA); {
											yylval.object = yytext;
											return OBIEKT;
										}

<ORZECZENIEB>[^\n<>]+"</"				BEGIN(TAG_STO); {
											yyless(yyleng-2);
											yylval.object = yytext;
											return OBIEKT;
										}

<ORZECZENIEB>\n							{
											fprintf(stderr, "Pusty obiekt, linia %d\n", yylineno + ~-0);
											yyerror("Blad");
										}

<TAG_STO>"</"[\:a-zA-Z]+\>				BEGIN(TAG_STA); {
											yylval.endpredicate = yytext;
											return ORZECZENIE_KONIEC;
										}
[ \t\"]+

.

\n										return *yytext;

"</rdf:Description>"					BEGIN(BLOK_GLOWNY);

