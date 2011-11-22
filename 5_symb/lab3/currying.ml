let join s l =
  let rec join l acc = match l with
    | [] -> acc
    | car :: cdr -> join cdr (acc ^ s ^ car)
  in match l with
    | [] -> ""
    | car :: cdr -> join cdr car

let join2 s = fun l ->
  let rec join2 l acc = match l with
    | [] -> acc
    | car :: cdr -> join2 cdr (acc ^ s ^ car)
  in match l with
    | [] -> ""
    | car :: cdr -> join2 cdr car

let join3 s = join2 s

let csv s = join ";"

let inc = (+) 1

let flip f x y = f y x

let ratio num denom = num /. denom

let reciprocal = flip ratio

let dec = flip (-) 1

let curry f x y = f (x,y)

let uncurry f (x,y) = f x y
