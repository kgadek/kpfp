(defparameter *simple-grammar*
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Noun))
    (verb-phrase -> (Verb noun-phrase))
    (Article -> the a)
    ;(Noun -> man ball woman table (Adjective Noun))
    (Noun -> man ball woman table)
    ;(Adjective -> big small nice beautiful)
    (Verb -> hit took saw liked)))

(defparameter *grammar* *simple-grammar*)

(defun rule-lhs (rule)
  (first rule))
(defun rule-rhs (rule)
  (rest (rest rule )))

(defun rewrites (category)
  (rule-rhs (assoc category *grammar*)))

(defun mappend (fn lst)
  (apply #'append (mapcar fn lst)))

(defun generate (phrase)
  (cond ((listp phrase)
	 (mappend #'generate phrase))
	((rewrites phrase)
	 (generate (random-elt (rewrites phrase))))
	(t
	 (list phrase))))

(defun random-elt (choices)
  (elt choices (random (length choices))))


(defparameter *bigger-grammar*
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Adj* Noun PP*) (Name) (Pronoun))
    (verb-phrase -> (Verb noun-phrase PP*))
    (PP* -> (PP PP*) ())
    (Adj* -> () (Adj Adj*))
    (PP -> (Prep noun-phrase))
    (Prep -> to in by with on)
    (Adj -> big little blue green adiabatic)
    (Article -> the a)
    (Name -> Pat Kim Lee Terry Robin House)
    (Noun -> man ball woman table)
    (Verb -> hit took saw liked)
    (Pronoun -> he she it these those that)))

(defparameter *aut-ex1-grammar*
  '((zdanie -> (grupa-podmiotu grupa-orzeczenia))
    (grupa-podmiotu -> (fraza-rzeczownikowa))
    (grupa-orzeczenia -> (fraza-czasownikowa))
    (fraza-rzeczownikowa -> (przymiotnik fraza-rzeczownikowa) (rzeczownik))
    (fraza-czasownikowa -> (czasownik fraza-przyslowkowa) (czasownik fraza-rzeczownikowa) (czasownik))
    (fraza-przyslowkowa -> (przyimek fraza-rzeczownikowa) (przyslowek) (przyslowek przyimek fraza-rzeczownikowa))
    (przymiotnik -> szybki rudy omszaly gleboki madry)
    (rzeczownik -> lis pien dol)
    (przyslowek -> zgrabnie bardzo)
    (przyimek -> przez)
    (czasownik -> przeskoczyl polubil)))
(setf *grammar* *aut-ex1-grammar*)     