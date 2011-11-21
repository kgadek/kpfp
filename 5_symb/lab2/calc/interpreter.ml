open Ast

exception Interpretation_error

type envType =
  | TFun
  | TVar
type envAtom =
  | Fun of (string list) * expr * (envAtom list)
  | Var of float

let emptyEnv () = []

let getVar env name =
  List.assoc (TVar, name) env
let rec setVar env name v = match env with
  | [] -> [((TVar, name), Var(v))]
  | ((TVar, k), Var(_)) :: tail when k = name ->
    ((TVar, k), Var(v)) :: tail
  | head :: tail -> head :: setVar tail name v

let getFun env name =
  List.assoc (TFun, name) env
let rec setFun env name args expr closure = match env with
  | [] -> [((TFun, name), Fun(args, expr, closure))]
    (* TODO: wyszukiwanie zależne od name+arg *)
  | ((TFun, k), Fun(_,_,_)) :: tail when k = name ->
    ((TFun, k), Fun(args, expr, closure)) :: tail
  | head :: tail -> head :: setFun tail name args expr closure

let interpret env stmt =
    print_string "Implement me\n"; env
