{
  open Parser
}
rule token = parse
    [' ' '\t'] { token lexbuf }
  | "def" { DEF }
  | "let" { LET }
  | '(' { LPAREN }
  | ')' { RPAREN }
  | ',' { COMMA }
  | '=' { EQ }
  | '+' { PLUS }
  | '-' { MINUS }
  | '*' { TIMES }
  | '/' { DIV }
  | ['0'-'9']+ ('.' ['0'-'9']*)? as lxm { FLOAT (float_of_string lxm) }
  | ['a'-'z']+ as lxm { ID lxm }
  | eof { EOF }
