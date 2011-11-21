%{
  open Ast
%}
%token EOF
%token DEF LET
%token LPAREN RPAREN
%token COMMA
%token EQ
%token PLUS MINUS TIMES DIV
%token <float> FLOAT
%token <string> ID
%left PLUS MINUS
%left TIMES DIV
%nonassoc UMINUS
%start stmt
%type <stmt> stmt
%type <string list> paramList nonemptyParamList
%type <expr> expr
%type <expr list> argList nonemptyArgList
%%
stmt:
    expr { Eval $1 }
  | LET ID EQ expr { Let ($2, $4) }
  | DEF ID LPAREN paramList RPAREN EQ expr { Def ($2, $4, $7) }
;
paramList:
    { [] }
  | nonemptyParamList { $1 }
;
nonemptyParamList:
    ID { [$1] }
  | ID COMMA nonemptyParamList { $1::$3 }
;
expr:
    FLOAT { Const $1 }
  | ID { Var $1 }
  | LPAREN expr RPAREN { $2 }
  | expr PLUS expr { Add ($1, $3) }
  | expr MINUS expr { Sub ($1, $3) }
  | expr TIMES expr { Mult ($1, $3) }
  | expr DIV expr { Div ($1, $3) }
  | MINUS expr %prec UMINUS { Minus $2 }
  | ID LPAREN argList RPAREN { Call ($1, $3) }
;
argList:
    { [] }
  | nonemptyArgList { $1 }
;
nonemptyArgList:
    expr { [$1] }
  | expr COMMA nonemptyArgList { $1::$3 }
;
