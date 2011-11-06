type expr =
  | Int of int
  | Float of float
  | Add of expr*expr
  | Sub of expr*expr
  | Mult of expr*expr
  | Div of expr*expr
  | Minus of expr;;
let rec eval ex = match ex with
  | Float(x) -> x
  | Int(x) -> float_of_int x
  | Add(a,b) -> (eval a) +. (eval b)
  | Sub(a,b) -> (eval a) -. (eval b)
  | Mult(a,b) -> (eval a) *. (eval b)
  | Div(a,b) -> (eval a) /. (eval b)
  | Minus(a) -> -. (eval a);;

(*
#load "expr.cmo";;
#load "lexer.cmo";;
#load "parser.cmo";;
#load "builder.cmo";;
open Builder;;
parse "1 + 2 + 3";;
let calc exstr = Expr.eval(parse exstr);;
*)
