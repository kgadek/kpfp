type nat =
  | O
  | S of nat

type 'a sig0 = 'a
  (* singleton inductive, whose constructor was exist *)

type bintree =
  | Leaf of nat
  | Node of bintree * bintree

(** val mirror_lemma : bintree -> bintree **)

let rec mirror_lemma = function
  | Leaf n -> Leaf n
  | Node (b0, b1) -> Node ((mirror_lemma b1), (mirror_lemma b0))

