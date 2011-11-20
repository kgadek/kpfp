let rec simpleReverse = function
  | [] -> []
  | car::cdr -> (simpleReverse cdr) @ [car];;
let rec reverseAccum lst acc = match lst with
  | [] -> acc
  | car::cdr -> reverseAccum cdr (car::acc);;

let reverseInt x =
  let rec revI x acc = match x with
    | 0 -> acc
    | x -> revI (x/10) (10*acc + x mod 10)
  in revI x 0;;
