%{
#define YYSTYPE char*

#include <iostream>
#include "Node.h"
using namespace std;

void yyerror(const char*);
int yylex();

Node *main;
%}

%union {
	Node *node;
	Name *name;
	Value *value;
	int token;
}

%token <string>
%token <Node>
%token <Name>
%token <Value>

%type <string>
%type <Node>
%type <Name>
%type <Value>


%%

all			:	

%%

int main() {
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    cerr << s;
}


