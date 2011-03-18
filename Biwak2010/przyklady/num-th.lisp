(defpackage "NUMBER-THEORY"
            (:use "COMMON-LISP")
            (:nicknames "NUM-TH")
            (:export "EGCD" "INVERT-MOD" "GENERATE-MOD" "CHINSKIE-TWIERDZENIE" "IN" "ROOT"
                     "RSA-CALCULATE-D" "RSA-ENCODE" "RSA-DECODE" "RABIN-ENCODE" "RABIN-DECODE"
                     "STIRLING-FIRST-KIND" "STIRLING-SECOND-KIND" "FACTORS"))
(in-package number-theory)

(defun egcd (x y)
  "Wyznacza największy wspólny dzielnik D liczb X i Y, oraz zwraca argumenty A i B: A*X+B*Y=D"
  (do ((m 1)
			 (n 0)
			 (o 0)
			 (p 1))
      ((= y 0) (values x m n))
      (multiple-value-bind (k d) (floor x y)
        (setf m (- m (* k o))
              n (- n (* k p)))
        (rotatef m o)
        (rotatef n p)
        (rotatef x y d))))
(defun root (x n &optional (eps 0.0001))
  "Zwraca N-ty pierwiastek liczby X"
  (do* ((p 0 c)
				(c 1 (float (/ (+ (* p (1- n))
													(/ x
														 (expt p (1- n))))
											 n))))
       ((< (abs (- p c))
					 eps)
				(float c))))
(defun invert-mod (x m)
  "Zwraca odwrotność liczby X modulo M"
  (multiple-value-bind (d r) (egcd x m)
    (if (= d 1)
			(mod r m))))
(defun generate-mod (x n)
  "Wyznacza grupę generowaną przez X modulo N"
  (do ((r nil)
			 (v x (mod (* x v) n)))
      ((or (and (= v x)
								(not (null r)))
					 (zerop v))
			 r)
      (push v r)))
(defun factors (x)
  "Wyznacza dzielniki pierwsze liczby X"
  (let ((r nil))
       (do ()
           ((plusp (mod x 2)))
           (push 2 r)
           (setf x (/ x 2)))
        (do ((i 3))
            ((= x 1))
            (if (zerop (mod x i))
                (progn (push i r)
                       (setf x (/ x i)))
                (incf i 2)))
        (nreverse r)))
(defun rsa-calculate-d (p q e)
  "Wyznacza liczbę d na podstawie podanych P, Q i E"
  (let* ((phi (* (1- p)
								 (1- q))))
         (invert-mod e phi)))
(defun rsa-decode (p q e c)
  "Funkcja dekodująca szyfru RSA"
  (mod (expt c
						 (rsa-calculate-d p q e))
			 (* p q)))
(defun rsa-encode (n e m)
  "Funkcja kodująca metodą RSA"
  (mod (expt m e)
			 n))
(defun chinskie-twierdzenie (lst)
  "Rozwiązuje układ kongruencji. Lista wejściowa powinna zawierać pary liczb odpowiadających Y_i oraz N_i."
  (let ((M (apply #'*
									(loop for i in lst collect (second i)))))
    (loop for i in lst sum
					(let* ((yi (second i))(Mi (floor M yi)))
						(* (third
								 (multiple-value-list (egcd yi Mi)))
							 Mi
							 (first i))))))
(defun rabin-encode (n ll)
  "Funkcja szyfrująca metodą Rabina"
  (mod (* ll ll)
			 n))
(defun rabin-decode (p q m)
  "Funkcja dekodująca szyfru Rabina. Zwraca listę 4 elementów, wśród których jeden jest tekstem jawnym"
  (let* ((a (mod (expt m (/ (1+ p) 4)) p))
         (b (- p a))
         (c (mod (expt m (/ (1+ q) 4)) q))
         (d (- q c))
         (n (* p q)))
    (mapcar #'(lambda(x) (mod x n))
      (mapcar #'chinskie-twierdzenie
        `(((,a ,p)(,c ,q))((,a ,p)(,d ,q))((,b ,p)(,c ,q))((,b ,p)(,d ,q)))))))
(defun in (x lst)
  "Stwierdza, czy X jest na liście LST"
  (plusp (loop for i in lst until (zerop i) count (= i x))))
(defun stirling-first-kind (n k)
  "Wyznacza liczbę Stirlinga I. rodzaju, tzn. liczbę sposobów na rozmieszczenie N liczb w K cyklach"
  (cond ((= k 0) (if (zerop n)
									 1
									 0))
        ((= n k) 1)
        ((> k n) 0)
        (t (+ (* (1- n)
								 (stirling-first-kind (1- n) k))
							(stirling-first-kind (1- n) (1- k))))))
(defun stirling-second-kind (n k)
  "Wyznacza liczbę Stirlinga II. rodzaju, tzn. liczbę sposobów podziału zbioru N elementowego na K niepustych podzbiorów"
  (cond ((= k 1) 1)
        ((= n k) 1)
        ((> k n) 0)
        (t (+ (* k
								 (stirling-second-kind (1- n) k))
							(stirling-second-kind (1- n) (1- k))))))

