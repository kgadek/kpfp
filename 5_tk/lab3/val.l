%option noyywrap

%{
#include <iostream>
#include <string>
#include "val.tab.h"
using namespace std;

#define TOKEN(t)    (yylval.token = t)

%}

%%

[a-zA-Z]+               return 

