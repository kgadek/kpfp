%{
/* Program wyszukuje ciąg znaków "lex" i zamienia go na ciąg znaków "flex" */
%}

%%

lex        printf("flex");

%%

int main() {
    printf("Zamiana lex na flex:\n");
    yylex();
    return 0;
}
