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

let rec eval env ex =
  match ex with
    | Const(v) -> v
    | Var(vn) -> let Bind(x) = getBind env vn in x
    | Add(e1, e2) -> (eval env e1) +. (eval env e2)
    | Sub(e1, e2) -> (eval env e1) -. (eval env e2)
    | Mult(e1, e2) -> (eval env e1) *. (eval env e2)
    | Div(e1, e2) -> (eval env e1) /. (eval env e2)
    | Minus(e1) -> 0. -. (eval env e1)
    | Call(fname, fexprs) ->
      let Fun(fargs,fbody) = getFun env fname in
      let env = (List.fold_left2 (fun e k v -> setBind e k (eval env v)) env fargs fexprs) in
      eval env fbody
      

let interpret env stmt = match stmt with
  | Eval(ex) -> Printf.printf "%f\n" (eval env ex); env
  | Let(k,v) -> setBind env k (eval env v)
  | Def(f,a,e) -> setFun env f a e
