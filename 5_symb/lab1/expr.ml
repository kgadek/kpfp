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
  | Add(a, b) when a = Int 0 || a = Float 0.0 -> simplify_expr(b)
  | Add(a, b) when b = Int 0 || b = Float 0.0 -> simplify_expr(a)
  | Mult(a, b) when a = Int 1 || a = Float 1.0 -> simplify_expr(b)
  | Mult(a, b) when b = Int 1 || b = Float 1.0 -> simplify_expr(a)
  | Add(a,b) -> (let ap = simplify_expr(a) and bp = simplify_expr(b) in
		 if ((a <> ap) || (b <> bp)) then
		   simplify_expr(Add(ap, bp))
		 else
		   Add(ap,bp))
  | Mult(a,b) -> (let ap = simplify_expr(a) and bp = simplify_expr(b) in
		  if (a <> ap) || (b <> bp) then
		    simplify_expr(Mult(ap, bp))
		  else
		    Mult(ap,bp))
  | Minus(Minus(a)) -> simplify_expr(a)
  | Minus(a) -> Minus(simplify_expr(a))
  | Div(a,b) -> Div(simplify_expr(a), simplify_expr(b))
  | Sub(a,b) -> Sub(simplify_expr(a), simplify_expr(b));;

let print_expr ex = (* the ugly one. But works ∀ my test cases *)
  let rec pr_expr ex = match ex with
    | Int(a) -> string_of_int a
    | Float(a) -> string_of_float a
    | Minus(Minus(a)) -> print_expr(a)
    | Minus(a) -> "-" ^ print_expr(a)
    | Add(a, Minus(b)) -> print_expr(a) ^ " - " ^ print_expr(b)
    | Sub(a, Minus(b)) -> print_expr(Add(a,b))
    | Sub(a, b) -> print_expr(a) ^ " - " ^ print_expr(b)
    | Mult((Add(a, b) as ab), (Add(c, d) as cd)) -> "( " ^ print_expr(ab) ^ " ) * ( " ^ print_expr(cd) ^ " )"
    | Mult((Add(b, c) as bc), a)
    | Mult(a, (Add(b, c) as bc)) -> print_expr(a) ^ " * (" ^ print_expr(bc) ^ ")"
    | Mult(a, b) -> print_expr(a) ^ " * " ^ print_expr(b)
    | Div((Add(a, b) as ab), (Add(c, d) as cd)) -> "( " ^ print_expr(ab) ^ " ) / ( " ^ print_expr(cd) ^ " )"
    | Div((Add(b, c) as bc), a)
    | Div(a, (Add(b, c) as bc)) -> print_expr(a) ^ " / (" ^ print_expr(bc) ^ ")"
    | Div(a, b) -> print_expr(a) ^ " / " ^ print_expr(b)
    | Add(a,b) -> print_expr(a) ^ " + " ^ print_expr(b)
  in pr_expr(simplify_expr(ex));;
