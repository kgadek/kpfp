(defpackage "SUDOKU"
  (:USE "COMMON-LISP")
  (:NICKNAMES "SUDOKU")
  (:EXPORT "SUDOKU" "DLX" "LOAD-DATA" "LOAD-KNUTH1"))
(in-package sudoku)

(defclass node ()
  ((R :accessor R :initform nil :initarg :R :type node)
   (L :accessor L :initform nil :initarg :L :type node)
   (U :accessor U :initform nil :initarg :U :type node)
   (D :accessor D :initform nil :initarg :D :type node)
   (C :accessor C :initform nil :initarg :C :type header)))
(defclass header (node)
  ((S :accessor S :initform 0 :initarg :S :type fixnum)
   (N :accessor N :initform "noname" :initarg :N :type string)))

(defparameter h (make-instance 'header :N 'h))
(setf (R h) h
      (L h) h)
(defparameter O nil)
(defparameter coltab nil)


(let ((res nil))
  (defun cover (c)
    (declare (optimize (speed 3) (safety 0)))
    (declare (type header c))
    (setf (L (R c)) (L c)
	  (R (L c)) (R c))
    (do ((i (D c) (D i)))
	((eq i c))
      (do ((j (R i) (R j)))
	  ((eq i j))
	(setf (U (D j)) (U j)
	      (D (U j)) (D j))
	(decf (the fixnum (S (C j)))))))
  
  (defun uncover (c)
    (declare (optimize (speed 3) (safety 0)))
    (declare (type header c))
    (do ((i (U c) (U i)))
	((eq i c))
      (do ((j (L i) (L j)))
	  ((eq j i))
	(incf (the fixnum (S (C j))))
	(setf (U (D j)) j
	      (D (U j)) j)))
    (setf (L (R c)) c
	  (R (L c)) c))
  
  (defun add-solution ()
    (push (loop for i in O collect
	       (cons (N (C i))
		     (do ((tmp nil)
			  (j (R i) (R j)))
			 ((eq i j) tmp)
		       (push (N (C j)) tmp))))
	  res))
	     
  (defun dlx-search (k)
    (declare (optimize (speed 3) (safety 0)))
    (declare (type fixnum k))
    (if (eq (R h) h)
	(add-solution)
	(let ((c (R h)))
	  (do ((c_cur c (R c_cur)))
	      ((eq c_cur h))
	    (when (< (the fixnum (S c_cur))
		     (the fixnum (S c)))
	      (setf c c_cur)))
	  (cover c)
	  (do ((r (D c) (D r)))
	      ((eq r c))
	    (push r O)
	    (do ((j (R r) (R j)))
		((eq j r))
	      (cover (C j)))
	    (dlx-search (the fixnum (+ k 1)))
	    (pop O)
	    (do ((j (L r) (L j)))
		((eq j r))
	      (uncover (C j))))
	  (uncover c))))

  (defun dlx ()
    (declare (optimize (speed 3) (safety 0)))
    (setf res nil)
    (dlx-search 0)
    res))

(defun load-data (pcol scol data)
  (let ((pcoln (length pcol))
	(scoln (length scol))
	(straznik (make-instance 'node
				 :L nil
				 :R nil
				 :U nil
				 :D nil
				 :C nil)))
    ; Ustaw nagłówek, tablicę wsk. na kolumny i wyczyść odpowiedzi
    (setf h (make-instance 'header)
	  (R h) h
	  (L h) h
	  O nil
	  coltab (make-array (+ pcoln scoln)
			     :initial-element (make-instance 'header :N "straznik")
			     :element-type 'header))
    ; Dodaj kolumny główne
    (do ((i 0 (1+ i))
	 (el pcol (rest el))
	 (popel h this)
	 (this nil))
	((null el))
      (setf this (make-instance 'header
				:N (first el)
				:L popel)
	    (R popel) this
	    (U this) this
	    (D this) this
	    (svref coltab i) this))
    (setf (L h) (svref coltab (1- pcoln))
	  (R (svref coltab (1- pcoln))) h)
    ; Dodaj kolumny dodatkowe
    (do ((i pcoln (1+ i))
	 (el scol (rest el))
	 (this nil))
	((null el))
      (setf this (make-instance 'header :N (first el))
	    (L this) this
	    (R this) this
	    (U this) this
	    (D this) this
	    (svref coltab i) this))
    ; Dodaj dane
    (dolist (row data)
      (let ((poprz straznik)
	    (c nil))
	(dolist (col row)
	  (setf c (svref coltab col)
		(U c) (make-instance 'node
				     :C c
				     :D c
				     :U (U c)
				     :L poprz
				     :R nil)
		(R poprz) (U c)
		(D (U (U c))) (U c)
		poprz (U c)))
	(setf (L (R straznik)) (U c)
	      (R (U c)) (R straznik))))))

(defun load-knuth1 ()
  (load-data '(A B C D E F G) nil
	     '((2 4 5)(0 3 6)(1 2 5)(0 3)(1 6)(3 4 6)))
  nil)

(defparameter sudoku1
  '(|53..7....|
    |6..195...|
    |.98....6.|
    |8...6...3|
    |4..8.3..1|
    |7...2...6|
    |.6....28.|
    |...419..5|
    |....8..79|))
(defparameter sudoku2
  '(|1....7.9.|
    |.3..2...8|
    |..96..5..|
    |..53..9..|
    |.1..8...2|
    |6....4...|
    |3......1.|
    |.4......7|
    |..7...3..|))

(defun sudoku-preparse (sym)
  (map 'list
       #'(lambda (c) (parse-integer (make-string 1 :initial-element c)
				    :junk-allowed t))
       (symbol-name sym)))

(defun box-num (k w)
  (+ (* 3 (truncate w 3))
     (truncate k 3)))

(let ((n-p 0))

  (defun sudoku-tab-pos (s x c)
    (let ((res (+ (* 9 x) c)))
      (case s
	(:k (incf res 81))
	(:w (incf res 162))
	(:b (incf res 243))
	(:p (incf res 0)))
      res))

  (defun sudoku (in)
    (let ((cols nil)
	  (cols-sec nil)
	  (dt-out nil)
	  (dt-in (make-array '(4 9 9) :element-type '(mod 2) :initial-element 0)))
      ;; Wczytaj dane
      (do ((w 0 (1+ w))
	   (line in (cdr line)))
	  ((or (>= w 9)
	       (null line)))
	(do ((k 0 (1+ k))
	     (data (sudoku-preparse (car line)) (rest data)))
	    ((or (>= k 9)
		 (null data)))
	  (let ((num (car data)))
	    (when (not (null num))
	      (decf num)
	      (if (plusp (+ (aref dt-in 0 k             num)
			    (aref dt-in 1 w             num)
			    (aref dt-in 2 (box-num w k) num)))
		  (error "Bledne dane wejsciowe!")
		  (setf (aref dt-in 0 k num)             1
			(aref dt-in 1 w num)             1
			(aref dt-in 2 (box-num w k) num) 1
			(aref dt-in 3 k w)               1))))))
      ;; Utworz kolumne podstawowa
      (loop for a from 8 downto 0 do
	   (loop for b from 8 downto 0 do
		(push (format nil "p_~Dx~D" (1+ a) (1+ b))
		      cols)))
      (print "gen-col-add")
      ;; Utworz kolumny dodatkowe
      (loop for s in '((:b 2) (:w 1) (:k 0)) do
	   (loop for x from 8 downto 0 do
		(loop for c from 8 downto 0 do
		     (push (format nil "~A_~D:~D" (car s) (1+ x) (1+ c))
			   cols-sec))))
      (print "gen-data")
      ;; Wygeneruj dane
      (loop for w from 8 downto 0 do
	   (loop for k from 8 downto 0
	      when (zerop (aref dt-in 3 k w)) do
		(loop for c from 8 downto 0
		   when (and (zerop (aref dt-in 0 k c))
			     (zerop (aref dt-in 1 w c))
			     (zerop (aref dt-in 2 (box-num k w) c))) do
		     (push (list (sudoku-tab-pos :p w k) ;; TU JEST BLAD !!!
				 (sudoku-tab-pos :k k c)
				 (sudoku-tab-pos :w w c)
				 (sudoku-tab-pos :b (box-num k w) c))
			   dt-out))))
      (loop for w from 8 downto 0 do
	   (loop for k from 8 downto 0
	      when (plusp (aref dt-in 3 k w)) do
		(push (list (sudoku-tab-pos :p w k)) dt-out)))
      (load-data cols cols-sec dt-out)
      (dlx))))