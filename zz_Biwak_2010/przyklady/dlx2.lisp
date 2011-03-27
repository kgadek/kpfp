(defclass node ()
  ((R :accessor R :initform nil :initarg :R)
   (L :accessor L :initform nil :initarg :L)
   (U :accessor U :initform nil :initarg :U)
   (D :accessor D :initform nil :initarg :D)
   (C :accessor C :initform nil :initarg :C)))
(defclass header (node)
  ((S :accessor S :initform 0 :initarg :S)
   (N :accessor N :initform "noname" :initarg :N)))

(defparameter h (make-instance 'header :N 'h))
(setf (R h) h
      (L h) h)
(defparameter O nil)

(defun print-curr-solution ()
  (loop for i in O do
       (format t "~A " (N (C i)))
       (do ((j (R i) (R j)))
	   ((eq i j))
	 (format t "~A " (N (C j))))
       (format t "| "))
  (format t "~%"))

(defun cover (c)
  (setf (L (R c)) (L c)
	(R (L c)) (R c))
  (do ((i (D c) (D i)))
      ((eq i c))
    (do ((j (R i) (R j)))
	((eq j i))
      (setf (U (D j)) (U j)
	    (D (U j)) (D j))
      (decf (S (C j))))))

(defun uncover (c)
  (do ((i (U c) (U i)))
      ((eq i c))
    (do ((j (L i) (L j)))
	((eq j i))
      (incf (S (C j)))
      (setf (U (D j)) j
	    (D (U j)) j)))
  (setf (L (R c)) c
	(R (L c)) c))

(defun btrace (k)
  (if (eq (R h) h)
      (print-curr-solution)
      (let ((c (R h)))
	(do ((c_cur c (R c_cur)))
	    ((eq c_cur h))
	  (when (< (S c_cur) (S c))
	    (setf c c_cur)))
	(cover c)
	(do ((r (D c) (D r)))
	    ((eq r c))
	  (push r O)
	  (do ((j (R r) (R j)))
	      ((eq j r))
	    (cover (C j)))
	  (btrace (+ k 1))
	  (pop O)
	  (do ((j (L r) (L j)))
	      ((eq j r))
	    (uncover (C j))))
  	(uncover c))))

(defun add-col (name)
  (setf (L h) (make-instance 'header :N name :R h :L (L h))
	(R (L (L h))) (L h)
	(U (L h)) (L h)
	(D (L h)) (L h)))

(defun add-data (inp)
  (let ((c (R h)) (le nil) (ri nil) (tmp nil))
    (loop for i in inp while (not (eq c h)) do
	 (when (plusp i)
	   (incf (S c))
	   (setf (U c) (make-instance 'node :U (U c) :D c :C c)
		 tmp (U c)
		 (D (U tmp)) tmp)
	   (when (null le)
	     (setf le tmp
		   ri tmp))
	   (setf (R le) tmp
		 (L ri) tmp
		 (R tmp) ri
		 (L tmp) le
		 le tmp))
	 (setf c (R c)))))

(defun load-example ()
  (loop for i in '(A B C D E F G) do (add-col i))
  (loop for i in '((0 0 1 0 1 1 0)
		   (1 0 0 1 0 0 1)
		   (0 1 1 0 0 1 0)
		   (1 0 0 1 0 0 0)
		   (0 1 0 0 0 0 1)
		   (0 0 0 1 1 0 1))
       do (add-data i)))