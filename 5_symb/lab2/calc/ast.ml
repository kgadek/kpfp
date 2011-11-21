type expr =
    Const of float
  | Var of string
  | Add of expr * expr
  | Sub of expr * expr
  | Mult of expr * expr
  | Div of expr * expr
  | Minus of expr
  | Call of string * (expr list)

type stmt =
    Eval of expr
  | Let of string * expr
  | Def of string * (string list) * expr
