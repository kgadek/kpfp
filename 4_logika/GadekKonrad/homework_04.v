(* Singleton *)

Inductive singleton_set : Set :=
  | selem.

Check singleton_set.
Check selem.
Check singleton_set_ind.

Theorem singleton_inhabitants : forall (x: singleton_set), x = selem.
induction x.
reflexivity.
Qed.

(* Zbior pusty *)

Inductive empty_set : Set := .

Check empty_set_ind.

Theorem wow : forall (x : empty_set), 2 + 2 = 7.
intro.
induction x.
Qed.

(* zmienne booleowskie *)

Inductive bool : Set :=
| true
| false.

Check bool_ind.

Definition not (b : bool) : bool :=
  match b with
    | true => false
    | false => true
  end.

Theorem not_not_bool : forall (b : bool), not (not b) = b.
induction b.
unfold not.
reflexivity.
unfold not.
reflexivity.
Qed.

Theorem not_not_bool' : forall (b : bool), not (not b) = b.
destruct b.
reflexivity.
reflexivity.
Qed.

Theorem not_ineq : forall (b : bool), not b <> b.
destruct b.
discriminate.
discriminate.
Qed.

(* Liczby naturalne *)

Inductive nat : Set :=
| O : nat
| S : nat -> nat.

Definition isZero (n : nat) : bool :=
  match n with
    | O => true
    | S _ => false
  end.

Definition pred (n : nat) : nat :=
  match n with
    | O => O
    | S n' => n'
  end.

Theorem succ_isZero : forall (n : nat), isZero (pred (S (S n))) = false.
destruct n;[reflexivity|reflexivity].
Qed.

Theorem succ_isZero' : forall (n : nat), isZero (pred (S (S n))) = false.
destruct n.
reflexivity.
reflexivity.
Qed.

Theorem succ_isZero'' : forall (n : nat), isZero (pred (S (S n))) = false.
destruct n.
unfold pred.
unfold isZero.
reflexivity.
unfold pred.
unfold isZero.
reflexivity.
Qed.

Fixpoint plus (n m : nat) : nat :=
  match n with
    | O => m
    | S n' => S (plus n' m)
  end.

Theorem O_plus_n : forall (n : nat), plus O n = n.
reflexivity.
Qed.

Theorem O_plus_n' : forall (n : nat), plus O n = n.
intro.
unfold plus.
reflexivity.
Qed.

Theorem n_plus_O : forall (n : nat), plus n O = n.
induction n.
reflexivity.
simpl.
rewrite IHn.
reflexivity.
Qed.

Check nat_ind.

(* Listy *)

Inductive list (T : Set) : Set :=
| Nil : list T
| Cons : T -> list T -> list T.

Fixpoint length T (ls : list T) : nat :=
  match ls with
    | Nil => O
    | Cons _  ls' => S (length  T ls')
  end.

Fixpoint concat T (ls1 ls2 : list T) : list T :=
  match ls1 with
    | Nil => ls2
    | Cons x ls1' => Cons T x (concat T ls1' ls2)
  end.

Theorem length_concat : 
  forall T (ls1 ls2 : list T), length T (concat T ls1 ls2) = plus (length T ls1) (length T ls2).

(* Zadanie domowe: uzupelnic dowod, powinno sie udac w 7 linijkach *)
intros.
induction ls1.
simpl.
trivial.
simpl.
rewrite IHls1.
trivial.
Qed.

