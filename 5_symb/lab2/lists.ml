let reverse lst =
  let rec reverse lst acc = match lst with
    | [] -> acc
    | car::cdr -> reverse cdr (car::acc)
  in reverse lst [];;

let head = function
  | [] -> invalid_arg "pusta lista"
  | car::cdr -> car;;

let tail lst =
  let rec tail lst acc = match lst with
    | [] -> acc
    | car::cdr -> tail cdr car
  in tail lst (head lst);;

let rec str lst = match lst with
  | [] -> []
  | car::cdr -> (string_of_int car)::(str cdr);;
let str2 lst =
  let rec str2 lst acc = match lst with
    | [] -> reverse acc
    | car::cdr -> str2 cdr ((string_of_int car)::acc)
  in str2 lst [];;
let str3 = List.map (fun x -> string_of_int x);;

let rec negatives = function
  | [] -> []
  | car::cdr when car<0 -> car::(negatives cdr)
  | car::cdr -> negatives cdr;;
let negatives2 lst =
  let rec negatives2 lst acc = match lst with
    | [] -> reverse acc
    | car::cdr when car<0 -> negatives2 cdr (car::acc)
    | car::cdr -> negatives2 cdr acc
  in negatives2 lst [];;
let negatives3 = List.filter (fun x -> x<0);;

let min lst =
  let rec min lst acc = match lst with
    | [] -> acc
    | car::cdr -> min cdr (if car < acc then car else acc)
  in min lst (head lst);;
let min2 lst = List.fold_left (fun x y -> if x < y then x else y) (head lst) lst;;

let capitalized lst =
  let rec capitalized lst acc = match lst with
    | [] -> acc
    | car::cdr -> capitalized cdr (let fst = String.get car 0
				   in if fst >= 'A' && fst <= 'Z'
				     then car::acc else acc)
  in capitalized lst [];;
let capitalized2 = List.filter (fun x -> let fst = (String.get x 0)
					 in if fst >= 'A' && fst <= 'Z'
					   then true else false);;

let rec firsts lst = match lst with
  | [] -> []
  | (a, b)::cdr -> a::firsts cdr;;
let firsts2 lst =
  let rec firsts2 lst acc = match lst with
    | [] -> reverse acc
    | (a, b)::cdr -> firsts2 cdr (a::acc)
  in firsts2 lst [];;
let firsts3 = List.map (fun (x,y) -> x);;

let rec append x y = match (x,y) with
  | (car::cdr,y) -> car::(append cdr y)
  | ([],y) -> y;;
let append2 x y =
  let rec append2 x y acc = match (x,y) with
    | (car::cdr,y) -> append2 cdr y (car::acc)
    | ([], car::cdr) -> append2 [] cdr (car::acc)
    | ([], []) -> reverse acc
  in append2 x y [];;

