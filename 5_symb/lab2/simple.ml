(* prosta silnia. O(x) / O(x) *)
let rec factorial x =
  if x < 2 then 1
  else x * factorial (x-1);;

(* silnia z TCO. O(x) / O(1) *)
let factorial_fast x =
  let rec fact x acc =
    if x < 2 then acc
    else fact (x-1) (x*acc)
  in fact x 1;;

(* proste int2bin. O(log^2 x) / O(log x) *)
let rec int2bin x =
  let ioio x =
    if x = 0 then "0" else "1"
  in
  if x < 2 then ioio x
  else String.concat "" [int2bin (x/2); ioio (x mod 2)];;

(* int2bin z TCO. O(log x) / O(log x) *)
let int2bin_fast x =
  let ioio x =
    if x = 0 then "0" else "1"
  in let rec i2b x lst =
       if x < 2 then ioio x :: lst
       else i2b (x/2) ((ioio (x mod 2)) :: lst)
     in String.concat "" (i2b x []);;

(* power. O(log n) / O(log n) *)
let rec power x n =
  if n = 0 then 1
  else
    let res = power x (n/2)
    in if n mod 2 == 0 then res*res
      else res*res*x;;

