(defpackage "SGA"
  (:use "COMMON-LISP" "SB-EXT")
  (:nicknames "SGA")
  (:export))
(in-package sga)

(defmacro defparameters (&rest rest)
  (do ((res nil)
       (i rest (cdr i)))
      ((null i) `(progn ,@(nreverse res)))
    (if (consp (car i))
	(push `(defparameter ,(caar i) ,(cadar i)) res)
	(push `(defparameter ,(car i) nil) res))))
(defmacro defsimpleclass (name abbrev &rest slots)
  `(defclass ,name nil (
		       ,@(mapcar #'(lambda(x)
				     `(,(car x)
					:accessor ,(intern (concatenate 'string (symbol-name abbrev) "-" (symbol-name (car x))))
					:initarg ,(intern (symbol-name (car x)) "KEYWORD")
					:initform ,(cadr x)))
				 slots))))

(defparameters
  (*maxpop* 100)
  (*maxstring* 30))
(defsimpleclass individual indiv
  (chrom (make-array *maxstring* :element-type 'boolean))
  (x 0.0)
  (fitness 0.0)
  (parent1 nil)
  (parent2 nil)
  (xsite nil))
(defparameters
  (*population* (make-array *maxpop* :element-type 'individual))
  *oldpop*
  *newpop*
  popsize
  lchrom
  gen
  maxgen
  pcross
  pmutation
  sumfutness
  nmutation
  ncross
  stat-avg
  stat-max
  stat-min)

(defun ag-select (popsize sumfitness pop)
  (do ((partsum 0.0 (+ partsum (indiv-fitness (nth j pop))))
       (j 0 (1+ j))
       (rand (random 1.0)))
      ((or (>= partsum rand)
	   (= j popsize))
       j)))
(defun flip (p)
  (< (random 1.0) p))
(defun rnd (a b)
  (+ a (random (- b a))))
(defun crossover (parent1 parent2 child1 child2 lchrom ncross nmutation jcross pcross pmutation)
  (if (flip pcross)
      (setf jcross (rnd 1 (1- lchrom))
	    ncross (1+ ncross))
      (setf jcross lchrom))
  (loop for j from 0 to (1- jcross) do
       (setf (nth j child1) (mutation (nth j parent1) pmutation nmutation)
	     (nth j child2) (mutation (nth j parent2) pmutation nmutation)))
  (when (/= jcross lchrom)
    (loop for j from jcross to (1- lchrom) do
	 (setf (nth j child1) (mutation (nth j parent2) pmutation nmutation)
	       (nth j child2) (mutation (nth j parent1) pmutation nmutation)))))
(defun mutation (&rest rest))