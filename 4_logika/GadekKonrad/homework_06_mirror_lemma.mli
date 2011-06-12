type nat =
  | O
  | S of nat

type 'a sig0 = 'a
  (* singleton inductive, whose constructor was exist *)

type bintree =
  | Leaf of nat
  | Node of bintree * bintree

val mirror_lemma : bintree -> bintree

