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

(defun onthelist? (el lst)
  (if (null lst)
      nil
      (or (eql el (car lst))
	  (onthelist? el (cdr lst)))))

;;; Kilka przykładowych pytań
(defquestion "Jak się masz?"
	     + "Dobrze"
	     "Tak sobie"
	     "Źleee")
(defquestion "Jak masz na imię?"
             "Igor"
             "Maks"
             "Radek"
             "Niebieski"
             + "Konrad")
(defquestion "\sqrt 2 jest równe:"
            "1"
            "2"
	    "3")
(defquestion "4 jest równe:"
    + "2^2"
    + "4"
    + "8/2"
    + "\sqrt 16")

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
	 (if (null pop)
	     (format t "Niepoprawne pytanie -- brak poprawnych odpowiedzi!~%~%~%")
	     (loop for z across (string (read))
		if (onthelist? z pop) do
		  (format t "~A -- poprawne~%" z)
		else do
		  (format t "~A -- NIEpoprawne!!~%" z)))
	 (format t "Poprawne: ~A~%~%~%~%" pop))))