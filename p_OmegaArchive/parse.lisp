(defun read-config (filename)
  "Wczytuje i wykonuje kod z podanego pliku."
  (with-open-file (conf filename)
    (loop for line = (read conf nil)
     while line
     do (eval line))))



;;;#############################################################################
;;; String & HTML
;;;#############################################################################

(defun string-split (str &optional (chr #\Space))
  "Dzieli string na tokeny wg znaku (domyślnie spacja)."
  (labels ((cheq (c) ; char-equal
         (and (graphic-char-p c)
          (char= c chr)))
       (split-int (str fun start) ; split-internal
         (let ((p1 (position-if-not fun str :start start)))
           (when p1
         (let ((p2 (position-if fun str :start p1)))
           (cons (subseq str p1 p2) 
             (when p2
               (split-int str fun p2))))))))
    (split-int str #'cheq 0)))
(defun join-string-list (lst)
  (apply #'concatenate (cons 'string lst)))

(defun mktagstr (name &key atts single-p closing-p)
  (concatenate 'string
	       "<" (when closing-p "/") (string-downcase name)
	       (format nil "~{~A~}" (mapcar #'(lambda(x) (format nil " ~A=\"~A\"" (car x) (cdr x)))
					    atts))
	       (when single-p "/")
	       ">"))
(defmacro tag (name atts &body body)
  `(concatenate 'string
		,(mktagstr name :atts atts)
		,@body
		,(mktagstr name :closing-p t)))
(defmacro :html (&body body)
  `(tag html ()
     ,@body))
(defmacro :head (&key (title "No-title") meta-2p)
  `(tag head ()
     ,@(mapcar #'(lambda(x) (format nil "<meta http-equiv=\"~A\" content=\"~A\">" (car x) (cadr x)))
	       meta-2p)
     (tag title () ',(string-downcase title))))
(defmacro :body (&body body)
  `(tag body ()
     ,@body))
(defmacro :strong (&body body)
  `(tag strong ()
     ,@body))
(defmacro :i (&body body)
  `(tag i ()
     ,@body))

(defun write-html-file (filename title cont)
  (with-open-file (out (concatenate 'string filename ".html")
		       :direction :OUTPUT
		       :if-exists :SUPERSEDE
		       :external-format :UTF-8)
    (format out
	    (:html
	     (:head :meta-2p (("Content-Type" "text/html; charset=utf-8"))
		    :title title)
	     (if (listp cont)
		 (join-string-list cont)
		 cont)))))



;;;#############################################################################
;;; Obsługa kontaktów
;;;#############################################################################
    
(defun get-contact-hist-file (contact server)
  (format nil "history/~A_at_~A.history" (string-downcase contact) (string-downcase server)))
(defun get-contact-out-file (contact server)
  (format nil "html//~A_at_~A.history" (string-downcase contact) (string-downcase server)))
(defun get-contact-display-name (contact server)
  (format nil "~A [~A]:" contact server))
(defun get-contact-html-title (contact server)
  (format nil "Rozmowy z ~A@~A" contact server))



;;;#############################################################################
;;; Main
;;;#############################################################################

(read-config "config.lisp")

(defun main (contact-name contact-server)
  (with-open-file (in (get-contact-hist-file contact-name contact-server)
		      :external-format :UTF-8)
    (write-html-file (get-contact-out-file contact-name
					   contact-server)
		     (get-contact-html-title contact-name
					     contact-server)
		      (loop for line = (read-line in
						  nil)
			 while line
			 collect
			   (let ((inp (string-split line
						    #\|)))
			     (format nil "~A ~A<br\>~%"
				     (if (string= (third inp) "to")
					 (:strong *user-self-name*)
					 (:strong (get-contact-display-name contact-name
									    contact-server)))
				     (fifth inp)))))))