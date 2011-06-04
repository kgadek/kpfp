(defun sv-binsearch (K vect size)
  (declare (fixnum K)
	   (simple-vector vect)
	   (fixnum size))
  (declare (optimize (compilation-speed 0)
		     (debug 0)
		     (safety 0)
		     (space 0)
		     (speed 3)))
  (let ((a 0))
    (declare (fixnum a))
    (decf size)
    (loop
       while (>= size a)
       for i = (floor (the fixnum (+ a size))
		      2)
       for val = (svref vect i)
       do
	 (if (>= K val)
	     (if (= K val)
		 (return-from sv-binsearch i)
		 (setf a (the fixnum (1+ i))))
	     (setf size (the fixnum (1- i)))))
    -1))

(defun testit()
  (let ((A (make-array 32 :initial-contents (loop for i below 32 collect i))))
    (time (loop
	     with max = 10000
	     with r = nil
	     for i below max
	     for j = (random max)
	     do
	       (loop
		  for k below max
		  do
		    (setf r (sv-binsearch j A 32)))))))
