(defun constituent (c)
  (lambda(x)
    (and (graphic-char-p x)
	 (not (char= x c)))))

(defun tokens (str test &key (start 0))
  (let ((p1 (position-if test str :start start)))
    (if p1 (let ((p2 (position-if #'(lambda(c)
				      (not (funcall test c)))
				  str :start p1)))
	     (cons (subseq str p1 p2) (if p2
					  (tokens str test :start p2)
					  nil)))
	nil)))

(defun main (filename)
  (with-open-file (in filename)
    (with-open-file (out (concatenate 'string filename ".html")
			 :direction :OUTPUT
			 :if-exists :SUPERSEDE)
      (format out "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Archiwum psi: ~A</title></head><body>~%" filename)
      (let ((constit (constituent #\|)))
	(loop for line = (read-line in nil)
	   until (eq line nil) do
	     (let ((inp (tokens line constit)))
	       (format out "~A ~A~A<br>~%"
		       (if (string= (third inp) "to")
			   "<strong>K: </strong><i>"
			   "<strong>M: </strong>")
		       (fifth inp)
		       (if (string= (third inp) "to")
			   "</i>"
			   "")))))
      (format out "</body></html>~%"))))