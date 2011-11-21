let parseStmt line =
  let lexbuf = Lexing.from_string line in
  Parser.stmt Lexer.token lexbuf
