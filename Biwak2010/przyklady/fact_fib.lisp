(defun factorial (x)
  (if (zerop x)
      1
      (* x (factorial (- x 1)))))
(defun fibA (n)
  (if (< n 2)
      n
      (+ (fibA (- n 1))
	 (fibA (- n 2)))))