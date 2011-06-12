Inductive bintree : Set :=
leaf : nat -> bintree
| node : bintree -> bintree -> bintree. 
 
(* 1. Sformułuj to pojęcie indukcyjnym predykatem *)
Inductive is_mirror : bintree -> bintree -> Prop :=
is_mirror_leaf: forall n : nat, is_mirror (leaf n) (leaf n)
| is_mirror_node: 
  forall a1 a2 b1 b2 : bintree,
    is_mirror a1 b2 -> is_mirror b1 a2 ->
    is_mirror (node a1 b1) (node a2 b2).

 
(* 2. Skonstruuj metodą ekstrakcji program zwracający obraz zwierciadłowy argumentu *)
Lemma mirror_lemma: forall tree : bintree, {mirror_tree : bintree | is_mirror tree mirror_tree}.
induction tree.
exists (leaf n).
apply is_mirror_leaf.
destruct IHtree1.
destruct IHtree2.
exists (node x0 x).
apply is_mirror_node.
assumption.
assumption.
Qed.
 
Recursive Extraction mirror_lemma.

 
(* 3. Zapisz rezultat *)
Cd "Studia/semestr 4/Logika/lab6".
Extraction "mirror_lemma.ml" mirror_lemma.

 
(* 4. Napisz program w Coq’u (Fixpoint) realizujący obraz zwierciadlany argumentu *)
Fixpoint mirror (tree : bintree) : bintree :=
match tree with
| leaf n => leaf n
| node left_tree right_tree => node (mirror right_tree) (mirror left_tree)
end.

 
(* 5. Przeprowadź dowód jego poprawności *)
Lemma mirror_program_dziala_lemma: forall tree : bintree, is_mirror tree (mirror tree).
induction tree.
simpl.
apply is_mirror_leaf.
simpl.
apply is_mirror_node.
assumption.
assumption.
Qed.
 
Extraction mirror.
 

(* 6. Wykonaj ekstracje tego programu i porównaj z rezultatem poprzedniej ekstrakcji *)
Cd "D:\coq\lab6".
Extraction "mirror.ml" mirror.

