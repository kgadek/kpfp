(defpackage :org.gadek.quicksort
  (:use :cl)
  (:export quicksort))
(in-package :org.gadek.quicksort)

(defun quicksort (A p r)
  "Sortuj tablicę A od elementu p do r włącznie"
  (when (< p r)
    (let ((q (partition A (1- p) p r (aref A r))))
      (quicksort A p (1- q))
      (quicksort A (1+ q) r))))

(defun partition (A 
i j r x)
  "Podziel tablicę A od elementu (i+1) do r włącznie względem elementu x. Przy wywołaniu
   j powinno mieć wartość (i+1)."
  (if (< j r)
      (progn
	(when (<= (aref A j)
		  x)
	  (rotatef (aref A (incf i))
		   (aref A j)))
	(partition A i (incf j) r x))
      (progn
	(rotatef (aref A (incf i))
		 (aref A r))
	i)))
