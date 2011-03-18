(defpackage :org.gadek.HansGen
  (:use :common-lisp)
  (:nicknames :hansGen)
  (:export main))

(in-package :org.gadek.HansGen)
(load "HansGen-Klienci.fasl")
(load "HansGen-Wycieczki.fasl")

(defconstant +il-Klienci+ 50)
(defconstant +il-Wycieczki+ 30)
(defconstant +il-Zamowienia+ 0)

(defun permutate-list! (lst &key len)
  (when (null len)
    (setf len (length lst)))
  (do ((i 0 (1+ i)))
      ((= i len) lst)
    (rotatef (nth i lst)
             (nth (+ i (random (- len i))) lst))))     

(defun flip (&optional (p 0.5))
  (< (random 1.0) p))

(defun choose-rnd (lst &key len)
  (when (null len)
    (setf len (length lst)))
  (nth (random len) lst))

(defun compose (&rest fns)
  ;; From ``On Lisp'', p.66
  (if fns
      (let ((fn1 (car (last fns)))
	    (fns (butlast fns)))
	#'(lambda (&rest args)
	    (reduce #'funcall fns
		    :from-end t
		    :initial-value (apply fn1 args))))
      #'identity))

(defun main()
  (gen-Klienci-prepare)
  (format t (gen-Klienci +il-Klienci+))
  (gen-Wycieczki-prepare)
  (format t (gen-Wycieczki +il-Wycieczki+))
  (format t "~%~%"))