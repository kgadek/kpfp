(defpackage :org.gadek.erwin
  (:use :common-lisp :sb-ext)
  (:export "RUN"))
(in-package :org.gadek.erwin)


;;; =============================================================================
;;; Makra ogolne ----------------------------------------------------------------
;;; =============================================================================

(defmacro defglobs (name &rest rest)
  "Key-saver. Zamiast wielu deklaracji mozna uzyc jednej, np
   (defglobs defparameter (a 1) b (c 2) d (e nil))
     <=>
   (defparameter a 1)
   (defparameter b nil)
   ..."
  (do ((res nil)
       (i rest (cdr i)))
      ((null i) `(progn ,@(nreverse res)))
    (if (consp (car i))
	(push `(,name ,(caar i) ,(cadar i)) res)
	(push `(,name ,(car i) nil) res))))

(defmacro defsimpleclass (name abbrev &rest slots)
  "Key-saver. Pozwala utworzyc prosta klase z odpowiednimi funkcjami dostepu."
  `(defclass ,name nil (
		       ,@(mapcar #'(lambda(x)
				     `(,(car x)
					:accessor ,(intern (concatenate 'string
									(symbol-name abbrev)
									"-"
									(symbol-name (car x))))
					:initarg ,(intern (symbol-name (car x)) "KEYWORD")
					:initform ,(cadr x)))
				 slots))))

(defmacro remember (set get)
  (let ((rem (gensym)))
    `(let ((,rem (progn ,get)))
       ,set
       ,rem)))


;;; =============================================================================
;;; Zmienne, stałe, struktury danych --------------------------------------------
;;; =============================================================================

(defglobs defconstant
    (+rozmiar-populacji+ 500)
    (+bitow-na-przedmiot+ 16)
    (+ilosc-iteracji+ 5)
    (+mutation-probability+ 0.001)
)

(defglobs defparameter
    *kryteria*
    *osoby*
    *przedmioty*
    *ilosc-osob*
    *ilosc-przedmiotow*
    *rozmiar-chromosomu*
)


(defsimpleclass individual indiv
  (chrom (make-array *rozmiar-chromosomu* :element-type '(mod 2)))
  (x 0.0)
  (fitness 0.0)
  (parent1 nil)
  (parent2 nil)
  (fit-fun nil)
)


;;; =============================================================================
;;; Przykładowe dane wejściowe --------------------------------------------------
;;; =============================================================================

(setf *osoby* '((:|John Doe|
		 (:|MOwNiT| 4 2 5)
		 (:|JTP II| 3 2 8 5)
		 (:|Bazy| 4))
		(:|Alan Parsons|
		 (:|JTP II| 3 2 2 3)
		 (:|MOwNiT| 3 2 2)
		 (:|Bazy| 3)))
      *przedmioty* '((:|MOwNiT| 3 0 10) ; przedmiot ilosc-grup min-osob max-osob
		     (:|JTP II| 4)
		     (:|Bazy| 1))
      *ilosc-osob* (length *osoby*)
      *ilosc-przedmiotow* (length *przedmioty*)
      *rozmiar-chromosomu* (* +bitow-na-przedmiot+
			      *ilosc-osob*
			      *ilosc-przedmiotow*))



;;; =============================================================================
;;; Funkcje ---------------------------------------------------------------------
;;; =============================================================================

(defun check-input ()
  (let ((przedmioty-ht (make-hash-table))
	(osoby-ht (make-hash-table)))
    (when (or
	   ;; Sprawdzanie przedmiotów
	   (some #'(lambda (p) (remember (setf (gethash (car p) przedmioty-ht) (cadr p))
					 (or (not (consp p)) ; musi być listą
					     (and (/= (length p) 2) ; musi mieć długość 2 lub 4
						  (/= (length p) 4))
					     (some (complement #'numberp) (cdr p)) ; parametry muszą być liczbami
					     (and (= (length p) 4) ; max-ilość-osób > min-ilość-osób
						  (> (caddr p) (cadddr p)))
					     (gethash (car p) przedmioty-ht)))) ; unikalna nazwa przedmiotu
		 *przedmioty*)
	   ;; Sprawdzanie osób
	   (some #'(lambda (o) (remember (setf (gethash (car o) osoby-ht) 1)
					 (or (not (consp o)) ; musi być listą
					     (/= (length o) (1+ *ilosc-przedmiotow*))
					         ; ilość współczynników odpowiada ilości przedmiotów
					     (gethash (car o) osoby-ht) ; osoba jest unikalna
					     (some #'(lambda (op) (or (not (consp op)) ; każdy wsp. to lista
								      (null (gethash (car op) przedmioty-ht))
								          ; każdy wsp. zaczyna się od nazwy przedmiotu
								      (/= (length op)
									  (1+ (gethash (car op) przedmioty-ht)))
								          ; odpowiednia ilość wsp. dla przedmiotu
								      (some #'(lambda (x) (or (not (numberp x))
											      (< x 0) ; liczba 0<x<10
											      (> x 10)))
									    (cdr op))))
						   (cdr o)))))
		 *osoby*)
	      (/= (length *przedmioty*) *ilosc-przedmiotow*))
      (error "Niepoprawny format danych wejsciowych!"))
    t))

(defun flip (p)
  "Wykonaj test Bernoulliego (rzut z moneta) z prawdopodobienstwem sukcesu p"
  (< (random 1.0) p))

(defun rnd (a b)
  "Losuj liczbe z przedzialu [a,b)"
  (+ a (random (- b a))))

(defun permutate-array! (array &key len)
  "Losowa permutacja tablicy"
  (when (null len)
    (setf len (length array)))
  (do ((i 0 (1+ i)))
      ((= i len) array)
    (rotatef (aref array i)
	     (aref array (+ i (random (- len i)))))))

(defun generate-random-chromosome (chrSize)
  "Wygeneruj losowy chromosom"
  (let ((res (make-array chrSize :element-type '(mod 2) :initial-element 0)))
    (loop for i from (1- chrSize) downto 0 when (flip 0.5) do (setf (bit res i) 1))
    res))

(defun generate-initial-population (count chrSize)
  "Wygeneruj populację początkową"
  (do ((res nil)
       (i (1- count) (1- i)))
      ((minusp i) (make-array count :initial-contents res :element-type 'individual))
    (push (make-instance 'individual :chrom (generate-random-chromosome chrSize))
	  res)))

(defun calculate-fitness! (pop count)
  "Oblicz wartość funkcji fitness dla całej populacji"
  (let ((sumFit 0.0)
	(elem nil))
    (dotimes (i count (float (/ sumFit count)))
      (setf elem (aref pop i))
      (setf (indiv-fitness elem) (fitness elem))
      (incf sumFit (indiv-fitness elem)))))

(defun reproduct (oldPop count)
  "Wygeneruj geny do reprodukcji"
  (let ((avgFit (calculate-fitness! oldPop count))
	(reproduct nil)
	(curr-cnt 0))
    (dotimes (i count)
      (dotimes (cnt (truncate (indiv-fitness (aref oldPop i)) avgFit))
	(push (aref oldPop i) reproduct)
	(incf curr-cnt)))
    (do ((toGo (- count curr-cnt))
	 (i 0 (mod (1+ i) count)))
	((zerop toGo) (make-array count :element-type 'individual :initial-contents reproduct))
      (when (flip (cadr (multiple-value-list (truncate (indiv-fitness (aref oldPop i)) avgFit))))
	(push (aref oldPop i) reproduct)
	(decf toGo)))))

(defun mutation (chrom chrSize)
  "Dokonaj mutacji"
  (dotimes (i chrSize)
    (when (flip +mutation-probability+)
      (if (plusp (sbit chrom i))
	  (setf (sbit chrom i) 0)
	  (setf (sbit chrom i) 1)))))

(defun crossover-single (x y n)
  "Operacja krzyżowania dwóch osobników"
  (values
   (make-instance 'individual :chrom (concatenate 'bit-vector (subseq x 0 n) (subseq y n)))
   (make-instance 'individual :chrom (concatenate 'bit-vector (subseq y 0 n) (subseq x n)))))

(defun crossover (reproduct count chrSize)
  "Operacja krzyżowania populacji"
  (let ((res nil))
    (setf reproduct (permutate-array! reproduct :len count))
    (multiple-value-bind (tms r) (truncate count 2)
      (dotimes (i tms)
	(multiple-value-bind (a b) (crossover-single (indiv-chrom (aref reproduct i))
						     (indiv-chrom (aref reproduct (1+ (* 2 i))))
						     (rnd 0 chrSize))
	  (push a res)
	  (push b res)))
      (when (plusp r)
	(push (aref reproduct (1- count)) res))
      (make-array count :element-type 'individual :initial-contents res))))	       

(defun fitness (x)
  "Funkcja przystosowania postaci: f(x) = x^2"
  (let ((res 0))
    (dotimes (i (length (indiv-chrom x)) (* res res))
      (incf res (+ res (bit (indiv-chrom x) i))))))

(let ((repeat-times 0)
      (default-rep 8))

  (defun reset-finalizer-test()
    "Resetuj moment końca wykonywania"
    (setf repeat-times default-rep))

  (defun set-finalizer-repetitions(rep)
    (setf default-rep rep))

  (defun good-enough? ()
    (decf repeat-times)
    (not (plusp repeat-times))))

(defun get-stats (pop count)
  "Odczytaj statystyki populacji"
  (let* ((best (aref pop 0))
	 (best-score (indiv-fitness (aref pop 0)))
	 (worst (aref pop 0))
	 (worst-score (indiv-fitness (aref pop 0)))
	 (sum-fitness 0.0)
	 (hashtab (make-hash-table)))
    (dotimes (i count
	      (values best best-score worst worst-score (float (/ sum-fitness count)) hashtab))
      (let* ((elem (aref pop i))
	     (elem-fit (indiv-fitness elem)))
	(incf sum-fitness elem-fit)
	(when (> elem-fit best-score)
	  (setf best-score elem-fit
		best elem))
	(when (< elem-fit worst-score)
	  (setf worst-score elem-fit
		worst elem))
	(setf (gethash elem hashtab) 1)))))

(defun run ()
  "Rozpocznij obliczenia"
  (reset-finalizer-test)
  (let ((stats nil))
    (labels ((push-curr-stats! (pop)
	       (calculate-fitness! pop +rozmiar-populacji+)
	       (push (multiple-value-list (get-stats pop +rozmiar-populacji+))
		     stats)))
      (do ((pop (generate-initial-population +rozmiar-populacji+ *rozmiar-chromosomu*)))
	  ((good-enough?) (nreverse (push-curr-stats! pop)))
	(push-curr-stats! pop)
	(setf pop (crossover (reproduct pop
					+rozmiar-populacji+)
			     +rozmiar-populacji+
			     *rozmiar-chromosomu*))))))