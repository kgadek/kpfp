(defpackage "RANDOMNESS"
  (:use "COMMON-LISP")
  (:nicknames "RAND")
  (:export "PERMUTATE"))
(in-package "RANDOMNESS")

(defun permutate (x &key len (rand-fun #'random))
  "Permutuje podany ciąg x. Opcjonalne parametry:
    len - długość łańcucha
    rand-fun - funkcja losowa (losuje liczby z przedziału [0,n) )"
  (when (null len)
    (setf len (length x)))
  (do ((i 0 (1+ i)))
      ((= i 5))
    (rotatef (nth i x)
	     (nth (+ i (funcall rand-fun (- len i))) x))))