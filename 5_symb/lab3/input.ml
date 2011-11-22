let diff_quotient f dx x = (f (x +. dx) -. f x) /. dx

let deriv f = diff_quotient f 0.000001

let (--) i j =
  let rec aux n acc = if n < i then acc else aux (n-1) (n::acc)
  in aux j []

let range i j d =
  let rec aux n acc = if n < i then acc else aux (n-.d) (n::acc)
  in aux j []

let integral f a b n =
  let dx = (b-.a)/.n
  in List.fold_left (+.) 0. (List.map (fun x -> dx *. (f x)) (range (a+.(dx/.2.)) (b-.(dx/.2.)) dx))
