%{
#include <cstdio>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <utility>

void yyerror(const char *);
int yylex();

std::string orzeczenieA;
std::string podmiot;
std::string orzeczenieB;

std::map<std::string, std::string> objects;
std::stack<std::string> tagi;
%}

%union {
char *subject;
char *predicate;
char *endpredicate;
char *object;
char *webpage;
}

%token	<webpage>		STRONA_WEB
%token	<subject>		PODMIOT
%token	<object>		OBIEKT
%token	<predicate>		ORZECZENIE
%token	<endpredicate>	ORZECZENIE_KONIEC

%token ENDTAG
%token ERR

%%

program		:
		 	| program '\n'
			| program wyrazenie '\n'
			;

wyrazenie	: PODMIOT					{ podmiot = $1; }
		  	| ORZECZENIE				{
											orzeczenieB = $1;
											tagi.push($1);
										}
			| ORZECZENIE_KONIEC				{
											printf("\n");
											std::string tmp = $1;
											if(tmp.substr(5, tmp.length()-6).compare(tagi.top()))
												fprintf(stderr,"Blad: Zle zamkniecie, na szczycie jest .%s.\n",
													tagi.top().c_str());
											tagi.pop();
										}
			| OBIEKT					{
											printf("podmiot:\t\t%s\norzeczenie:\t\t%s%s\n",
												podmiot.c_str(), orzeczenieA.c_str(), orzeczenieB.c_str());
											std::string tmp = podmiot + "_" + orzeczenieA + orzeczenieB;
											if(objects.find(tmp) != objects.end())
												fprintf(stderr,"Blad! Orzeczenie %s%s juz zdefiniowano\n",
													orzeczenieA.c_str(), orzeczenieB.c_str());
											else
												objects.insert(std::pair<std::string, std::string>(tmp, $1));
											printf("obiekt:\t\t\t%s\n",$1);
										}
			| STRONA_WEB					{ orzeczenieA = $1; }
			| wyrazenie wyrazenie
			;

%%

int main() {
	yyparse();
	return 0;
}

void yyerror(const char *s) {
	fprintf(stderr, "Blad: %s\n", s);
}

