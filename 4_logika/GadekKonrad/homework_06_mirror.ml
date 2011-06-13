type nat =
  | O
  | S of nat

type bintree =
  | Leaf of nat
  | Node of bintree * bintree

(** val mirror : bintree -> bintree **)

let rec mirror = function
  | Leaf n -> Leaf n
  | Node (left_tree, right_tree) -> Node ((mirror right_tree),
      (mirror left_tree))

