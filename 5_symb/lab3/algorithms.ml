let flip f x y = f y x

let curry f x y = f (x,y)

let uncurry f (x,y) = f x y

let rec map f l = match l with
  | [] -> []
  | car :: cdr -> (f car) :: map f cdr

let map2 f l =
  let rec map2 l acc = match l with
    | [] -> List.rev acc
    | car :: cdr -> map2 cdr (car :: acc)
  in map2 l []

let rec filter f l = match l with
  | [] -> []
  | car :: cdr when (f car) -> car :: filter f cdr
  | _ :: cdr -> filter f cdr

let filter2 f l =
  let rec filter2 l acc = match l with
    | [] -> List.rev acc
    | car :: cdr when (f car) -> filter2 cdr (car :: acc)
    | _ :: cdr -> filter2 cdr acc
  in filter2 l []

let rec fold_left f acc l = match l with
  | [] -> acc
  | car :: cdr -> fold_left f (f acc car) cdr

let fold_right f l acc = fold_left (flip f) acc (List.rev l)

let filter3 f l = fold_right (fun x y -> if (f x) then x::y else y) l []

let head l = match l with
  | [] -> invalid_arg "Pusta głowa!"
  | car :: cdr -> fold_left (fun x y -> x) car cdr (* waat? *)

let last l = match l with
  | [] -> invalid_arg "Pusty ogon!"
  | car :: cdr -> fold_left (fun x y -> y) car cdr

let str = map (fun x -> string_of_int x)

let negatives = filter (fun x -> x<0)

let min lst = fold_left (fun x y -> if x < y then x else y) (head lst) lst

let capitalized = filter (fun x -> let fst = (String.get x 0)
                                   in if fst >= 'A' && fst <= 'Z'
                                     then true else false)

let firsts = List.map (fun (x,y) -> x)

let append la lb = List.rev (fold_left (fun x y -> y :: x) (List.rev la) lb)

let rec map2 f l1 l2 = match (l1,l2) with
  | ([], []) -> []
  | (h1::t1, h2::t2) -> (f h1 h2) :: map2 f t1 t2
  | (_, _) -> invalid_arg "Listy nie są równej długości"

let fold_left2 f acc l1 l2 =
  let rec fl2 acc l1 l2 = match (l1,l2) with
    | ([], []) -> List.rev acc
    | (h1::t1, h2::t2) -> fl2 ((h1,h2)::acc) t1 t2
  in fold_left (fun x (y,z) -> f x y z) acc (fl2 [] l1 l2)

let vadd = map2 (+)

let vmul = fold_left2 (fun x y z -> x+y*z) 0

let for_all f = fold_left (fun x y -> x && (f y)) true

let combine l1 l2 = List.rev (fold_left2 (fun x y z -> (y,z)::x) [] l1 l2)
