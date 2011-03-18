;;;; Program szuka ciągu przekształceń (ze zbioru funstack) na danych wejściowych
;;;; xin takich, by uzyskać yout.
;;;;
;;;; Program miał odkryć ,,tajne" przekształcenie na ciągu kolejnych
;;;; liczb naturalnych, które generuje pewne konkretne kody ASCII.
;;;; Nie znalazł ich... A później okazało się, że kody (znaki) były
;;;; wybierane ręcznie


(declaim (optimize (speed 3) (compilation-speed 0) (safety 0) (debug 0)))
(defparameter xin  '( 1   2  3  4  5  6  7  8  9 10 11 12 13 14))
(defparameter yout '(46 111 43 61 42 66 79 88 64 37 38 35 47 94))
;(defparameter yout '(1 4 9 16 25 36 49 64 81 100 121 144 169 196))
(defparameter modder 256)

(defvar funstack nil)
(defvar resstack nil)
(defvar tmpstack nil)
(defvar found nil)
(defvar it 0)

(defun operate (r f)
	(list (loop for i in (first r) collect (mod (funcall f i) modder)) (append (second r)(list f))))

(defun op_kwadrat (x)
  (mod 	(expt x 2) modder))
(push #'op_kwadrat funstack)
(defun op_szescian (x)
  (mod 	(expt x 3) modder))
(push #'op_szescian funstack)
(defun op_shl (x) 
  (mod 	(+ (* x 2) (floor (boole boole-and x 128) 128)) modder))
(push #'op_shl funstack)
(defun op_shr (x)
	(mod (+ (floor x 2) (* (boole boole-and x 1) 128)) modder))
(push #'op_shr funstack)
(defun op_x2 (x)
	(mod (* x 2) modder))
(push #'op_x2 funstack)
(defun op_d2 (x)
	(mod (floor x 2) modder))
(push #'op_d2 funstack)

(push (list xin nil) resstack)

(defmacro while (con &body body)
	`(do ()
		((not ,con))
		,@body))
(defmacro ifthen (con &body body)
	`(if ,con (progn ,@body)))

(let ((tmpA nil) (tmpB nil))
  (while (not found)
				 (format t "Iteracja ~D, rozmiar stosu ~D~%" (incf it) (length resstack))
  			 (while (not (endp resstack))
								(setf tmpA (pop resstack))
								(loop for i in funstack do
											(setf tmpB (operate tmpA i))
											(ifthen (equal (first tmpB) yout)
															(format t "~D~%" tmpB)
															(setf found t))
											(push tmpB tmpstack)))
				 (format t "Cleaning...~%")
				 (setf resstack (remove-duplicates tmpstack :test (lambda (a b) (equal (first a) (first b)))))
				 (setf tmpstack nil)))

