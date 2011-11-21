open Ast

exception Interpretation_error

type envType =
  | TFun
  | TBind
type envAtom =
  | Fun of (string list) * expr
  | Bind of float

let emptyEnv () = []

let getBind env name =
  List.assoc (TBind, name) env
let rec setBind env name v = match env with
  | [] -> [((TBind, name), Bind(v))]
  | ((TBind, k), Bind(_)) :: tail when k = name ->
    ((TBind, k), Bind(v)) :: tail
  | head :: tail -> head :: setBind tail name v

let getFun env name =
  List.assoc (TFun, name) env
let rec setFun env name args expr = match env with
  | [] -> [((TFun, name), Fun(args, expr))]
    (* TODO: wyszukiwanie zależne od name+arg *)
  | ((TFun, k), Fun(_,_)) :: tail when k = name ->
    ((TFun, k), Fun(args, expr)) :: tail
  | head :: tail -> head :: setFun tail name args expr

let rec eval env expr =
  match expr with
    | Const(v) -> v
    | Var(vn) -> getBind env vn
    | Add(e1, e2) -> (eval env e1) +. (eval env e2)
    | Sub(e1, e2) -> (eval env e1) -. (eval env e2)
    | Mult(e1, e2) -> (eval env e1) *. (eval env e2)
    | Div(e1, e2) -> (eval env e1) /. (eval env e2)
    | Minus(e1) -> 0. -. (eval env e1)
    | Call(f, body) ->
      let Fun(a,b) = (getFun env f) in 5.
      (*let Fun(f, expr list ) = getFun env f in *)
      (*let Fun(f_a, f_e) = getFun env f in*)
      (*let env = List.fold_left2 setBind env f_a exprs in*)
	
      
	

let interpret env stmt =
  print_string (string_of_float (eval env expr)); env
  (* match stmt with
  | Eval(expr) -> print_string (string_of_float (eval env expr)); env
  | Let(k,v) -> setBind env k (eval env v)
  | Def(f, a, e) -> setFun env f a e *)
