;;;; Program liczący czas i kolejność wysłania ataku z zadanych miast
;;;;
;;;; Wczytaj godzinę docelową (hh mm ss), ilość miast (N) a następnie N opisów czasu
;;;; dotarcia floty z miasta i do miasta docelowego (hh mm ss nazwa_miasta_początkowego).

(defun t->s (h m s) (+ s (* 60 m) (* 3600 h)))
(defun s->t (s) (list (floor (mod s (* 3600 24)) 3600) (floor (mod s 3600) 60) (mod s 60)))
(format t "Podaj godzinę docelową (hh mm ss), następnie liczbę N - ilość danych wejściowych~%a następnie N linii: hh mm ss nazwa_miasta~%")
(defparameter t0 (t->s (read) (read) (read)))
(format t "~{~{~A - ~A~}~%~}"
        (mapcar #'(lambda(x) (let ((r (s->t (car x)))) (list (format nil "~2,'0D:~2,'0D:~2,'0D" (first r) (second r) (third r)) (second x))))
                (sort   (loop for i from (read) downto 1 collect
                          (list (+ (- t0 (t->s (read) (read) (read))) 86400) (read)))
                        #'(lambda (x y) (< (car x) (car y))))))
