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

let rec simplify_expr ex = match ex with
  | Float(x) -> Float x
  | Int(x) -> Int x
  | Add(Int 0, a)
  | Add(Float 0.0, a)
  | Add(a, Int 0)
  | Add(a, Float 0.0) -> simplify_expr(a)
  | Mult(Int 1, a)
  | Mult(Float 1.0, a)
  | Mult(a, Int 1)
  | Mult(a, Float 1.0) -> simplify_expr(a)
  | Add(a,b) -> Add(simplify_expr(a), simplify_expr(b))
  | Mult(a,b) -> Mult(simplify_expr(a), simplify_expr(b))
  | Minus(a) -> Minus(simplify_expr(a))
  | Div(a,b) -> Div(simplify_expr(a), simplify_expr(b))
  | Sub(a,b) -> Sub(simplify_expr(a), simplify_expr(b));;
