type 'a btree =
  | Nil
  | Node of 'a * 'a btree * 'a btree;;

let rec insert t x = match t with
  | Nil -> Node (x, Nil, Nil)
  | Node (v, l, r) ->
    if x<=v then Node(v, insert l x, r)
    else Node(v, l, insert r x);;

let buildTree1 () =
  Node(5, Node(3, Node(1, Nil, Nil), Node(4, Nil, Nil)), Node(8, Nil, Nil));;
let buildTree2 () =
  insert (insert (insert (insert (insert Nil 5) 3) 1) 4) 8;;

let rec member t x = match t with
  | Nil -> false
  | Node (v, l, r) ->
    if x=v then true
    else if x<=v then member l x
    else member r x;;
let rec keys = function
  | Nil -> []
  | Node (v, l, r) -> (keys l) @ v::(keys r);;
let rec height = function
  | Nil -> 0
  | Node (v, l, r) ->
    let x = height l and y = height r
    in (if x>y then x else y)+1;;
let rec size = function
  | Nil -> 0
  | Node (v, l, r) -> 1+(size l)+(size r);;
