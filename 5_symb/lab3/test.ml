let rec fold_right f lst acc = match lst with
  | [] -> acc
  | head::tail -> f head (fold_right f tail acc);;
let filter f lst = fold_right (fun x y -> if (f x) then x::y else y) lst [];;
