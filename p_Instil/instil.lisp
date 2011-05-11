(defparameter *questions* nil)

(defun add-question (q ans-lst)
  (push (cons q (loop for prev = i
		   and i in ans-lst
		   when (not (eql i '+))
		   if (eql prev '+)
		     collect (list i)
		   else
		     collect i))
	*questions*))
(defmacro defquestion (q &rest rest)
  `(add-question ,q ',rest))
(defun unique (lst)
  (loop for i in lst
     when (not (position i res))
     collect i into res
     finally (return res)))

(load "questions.lisp")
;;; Kilka przykładowych pytań

(defun przepytaj ()
  (loop for z in *questions* do

       (let ((q (car z)) ; pytanie
	     (a (cdr z)) ; zbiór odpowiedzi
	     (pop nil)) ; zbiór poprawnych odpowiedzi (lista liter)
	 
	 (format t "Pytanie: ~A~%-----------------------------------~%~{~A) ~A~%~}~%~%"
		 q ; pytanie
		 (loop for id = 65 then (1+ id) ; ascii(65) == 'A'
		    and odpowiedz-n in a
		    if (listp odpowiedz-n)
		      do (push (code-char id) ; jeśli poprawne -- dodaj literę do zbioru poprawnych
			       pop)
		    end
		    append (list (code-char id) 
				 (if (listp odpowiedz-n) ; odpowiedzi poprawne są listą jednoelementową...
				     (car odpowiedz-n) ; ...więc czasem trzeba je wyłuskać...
				     odpowiedz-n)))) ; ...a czasem nie
	 (setf pop (nreverse pop)) ; odpowiedzi są wrzucone wspak -- poprawiamy
	 (if (null pop)
	     (format t "Niepoprawne pytanie -- brak poprawnych odpowiedzi!~%~%~%")
	     (let ((inp (unique
			 (loop for i across (string (read)) ; wczytaj odpowiedź
			    collect i))))
	       (let ((zapomniane (set-difference pop ; wynik odpowiedzi
						 inp))
		     (zle (set-difference inp
					  pop)))
		 (when zapomniane
		   (format t "## Brakujące odpowiedzi: ~{~A~^, ~}~%" zapomniane))
		 (when zle
		   (format t "## Niepoprawne odpowiedzi: ~{~A~^, ~}~%" zle))
		 (when (not (or zle
				zapomniane))
		   (format t "## OK! :)~%")))
	       (format t "Poprawne: ~{~A~^, ~}~%~%~%~%" pop)))
	 (format t "Kontynuować? [yn]~%")
	 (if (eql (loop for ans = (read)
		     while ans
		     if (or (eql ans 'y)
			    (eql ans 'n))
		     return ans)
		  'n)
	     (return nil)))))