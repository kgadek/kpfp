type nat =
  | O
  | S of nat

type bintree =
  | Leaf of nat
  | Node of bintree * bintree

val mirror : bintree -> bintree

