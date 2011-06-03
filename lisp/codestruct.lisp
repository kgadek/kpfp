(defmacro mx1 (blah)
  "Makro przydatne do pisania makr :)"
  `(pprint (macroexpand-1 ',blah)))

(defmacro for (from cmp1 var cmp2 upto &body body)
  `(loop
      for ,var from ,(cond ((equal cmp1 '<) (1+ from))
			   (t from))
	,(cond ((equal cmp2 '<) 'below)
	       (t 'to))
      ,upto do (progn ,@body)))

(defmacro dahl-loop (&body body)
  (let ((pos-while (position 'while body))
	(blockname (gensym)))
    `(block ,blockname
       (loop
	  (progn ,@(subseq body
			   0
			   pos-while))
	  (if ,(elt body
		    (1+ pos-while))
	      (progn ,@(subseq body
			       (+ pos-while 2)))
	      (return-from ,blockname))))))

(defmacro event-loop (evts &body body)
  (let ((blockname (gensym)))
    `(block ,blockname
       (flet ,(loop
		 for evt in evts
		 for divisor = (position '=> evt)
		 collect `(,(car evt)
			    ,(loop
				for i in (subseq evt 1 divisor)
				collect i)
			    (return-from ,blockname
			      (progn ,@(subseq evt
					       (1+ divisor))))))
	 (loop
	      (progn ,@body))))))

(defmacro event-body (evts &body body)
  (let ((blockname (gensym)))
    `(block ,blockname
       (flet ,(loop
		 for evt in evts
		 for divisor = (position '=> evt)
		 collect `(,(car evt)
			    ,(loop
				for i in (subseq evt 1 divisor)
				collect i)
			    (return-from ,blockname
			      (progn ,@(subseq evt (1+ divisor))))))
	 (progn ,@body)
	 (error "Błąd bloku event-body -- nie wystąpiło żadne zadeklaroweane zdarzenie")))))

#|(defmacro iterate  (test &body body)
  (let ((blockname (gensym))
	(tmp (gensym)))
    `(block ,blockname
       (flet ((finish-while (&optional val) (return-from ,blockname val)))
	 (do ((,tmp nil))
	     (tmp)
	 (loop while ,test do
	   ,@body))))))|#
#+nil(defmacro named-when (name test &body body)
  (let ((blockname (gensym))
	(funname (concatenate 'string "DROPOUT-" (string name))))
    `(block ,blockname
       (labels ((,funname (&optional val)
		  (return-from ,blockname val))
		(dropout (&optional val)
		  (,funname val)))
	 (loop
	    while ,test
	    do  (progn
		  ,@body))))))

"Propozycja struktury (C.T. Zahn) pętli. Pętla wykonuje się aż do wystąpienia jednego z podanych wydarzeń. Wydarzenia podaje się w formie:
  (event --> code)
lub
  (event (param1 param2 ... paramN) code).

Przykład:
(defparameter A '(1 2 3 4 5 6 7 8 9 10))
(defparameter x 6)
(loop-until-event ((found =>
			  (format t \"Znalazłem: ~A~%\" A)
			  A)
		   (empty => nil))
   (when (equal (car A)
		x)
     (found))
   (when (null A)
     (empty))
   (setf A (cdr A)))"