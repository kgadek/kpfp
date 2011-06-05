(declaim (optimize (speed 3) (debug 0) (safety 0) (compilation-speed 0)))

(defun sv-binsearch (K vect size)
  (declare (fixnum K)
	   (type (simple-array fixnum (32)) vect)
	   (fixnum size))
  (decf size)
  (loop
     with a of-type fixnum = 0
     while (>= size a)
     for i of-type fixnum = (floor (the fixnum (+ a size))
				   2)
     for val of-type fixnum = (aref vect i)
     do
       (if (>= K val)
	   (if (= K val)
	       (return-from sv-binsearch i)
	       (setf a (the fixnum (1+ i))))
	   (setf size (the fixnum (1- i)))))
  -1)

(defun testit()
  (let ((A (make-array 32 :element-type 'fixnum :initial-contents (loop for i below 32 collect i))))
    (time (loop
	     with max of-type fixnum = 10000
	     with r of-type fixnum = nil
	     for i of-type fixnum below max
	     for j of-type fixnum = (random max)
	     do
	       (loop
		  for k of-type fixnum below max
		  do
		    (setf r (the fixnum (sv-binsearch j A 32))))))))
