(declaim (optimize (speed 3) (debug 0) (safety 0) (compilation-speed 0)))
#+nil(ql:quickload 'mt19937)
(asdf:oos 'asdf:load-op :mt19937)

(defun sv-binsearch (K vect a b)
  (declare (fixnum K a b)
	   (type (simple-array fixnum (32)) vect)
	   (optimize (speed 3) (debug 0) (safety 0) (compilation-speed 0)))
  (if (< b a)
      -1
      (let* ((i (floor (+ a b)
		       2))
	     (val (aref vect i)))
	(declare (fixnum i val))
	(cond ((> K val) (sv-binsearch K vect
				       (1+ i)
				       b))
	      ((< K val) (sv-binsearch K vect
				       a
				       (1- i)))
	      (t i)))))

(defun testit()
  (let ((A (make-array 32 :element-type 'fixnum :initial-contents (loop for i below 32 collect i))))
    (time (loop
	     with max of-type fixnum = 10000
	     with r of-type fixnum = nil
	     for i of-type fixnum below max
	     for j of-type fixnum = (mt19937:random 32)
	     do
	       (loop
		  for k of-type fixnum below max
		  do
		    (setf r (the fixnum
			      (sv-binsearch j A 0 31))))))))
