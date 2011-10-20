let x = 5 and
    y = 5. and
    addii x y = x + y and
    addff x y = x +. y and
    addif x y = (float x) +. y and
    abs x = if x < 0 then -x else x and
    sgn x = if x < 0. then (-1) else if x > 0. then 1 else 0 and
    fib n = let rec fibacc a b n = (* fibonacci działający w czasie O(n) i pamięci O(1) :D *)
	      if n = 0
	      then a
	      else fibacc b (a+b) (n-1) in
	    fibacc 0 1 n;;
type vector = float * float;;
type transformation =
  | Sx
  | Sy
  | So
  | T of float*float (* of vector ?? *)
  | Jo of float;;
let transform t (x,y) = match t with
  | Sx -> (x, -.y)
  | Sy -> (-.x, y)
  | So -> (-.x, -.y)
  | T(a,b) -> (x+.a, b+.y)
  | Jo(s) -> (s*.x, s*.y);;
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

open Builder;;
open Lexer;;
open Parser;;
