(in-package :org.gadek.HansGen)

(defparameter *Wycieczki-table*
  "ID_kraju, Nazwa_pensjonatu, Adres_pensjonatu, Cena, Liczba_miejsc, Data_wyjazdu, Data_przyjazdu")
(defparameter *Kraje-table*
  "Nazwa_kraju")
(defparameter *Dodatki-table*
  "ID_wycieczki, Opis_dodatku, Cena_dodatku, Liczba_miejsc, Termin_dodatku")

(defparameter wycieczki
  '((|Egipt| |Hotel Gardenia Plaza Resort| |Zatoka Naama Bay| 1037 75 |2011-02-19| |2011-02-26|
     ((|Dolina Nilu|
       |Wycieczka do <strong>doliny Nilu</strong> z naszymi wspaniałymi przewodnikami|
       100 10 |2011-01-23|)))
    (|Egipt| |Hotel Empire| |Dzielnica El Dahar| 1037 75 |2011-02-28| |2011-03-04|
     ((|Dolina Nilu|
       |Wycieczka do <strong>doliny Nilu</strong> z naszymi wspaniałymi przewodnikami|
       100 10 |2011-01-31|)))
    (|Egipt| |Hotel Resta Sharm| |Dzielnica Hadaba| 1089 50 |2011-02-18| |2011-02-25|
     ((|Dolina Nilu|
       |Wycieczka do <strong>doliny Nilu</strong> z naszymi wspaniałymi przewodnikami|
       100 10 |2011-01-23|)))
    (|Egipt| |Hotel Nubian Village| |Zatoka Nabq Bay| 6924 20 |2011-01-18| |2011-02-15|
     ((|Dolina Nilu|
       |Wycieczka do <strong>doliny Nilu</strong> z naszymi wspaniałymi przewodnikami|
       100 10 |2011-01-25|)))
    (|Turcja| |Fatih Hotel| |Alanya| 1290 40 |2011-05-11| |2011-05-18| ())
    (|Grecja| |Candia| |Ateny, centrum miasta| 2036 50 |2011-05-12| |2011-05-15|
     ((|Centrum Aten|
       |Zwiedzanie miasta wraz z przewodnikiem, historia Aten, ciekawostki|
       50 50 |2011-05-13|)
      (|Akropol|
       |Właściwie <em>pozycja obowiązkowa</em> dla każdego, kto jeszcze nie widział Akropolu z bliska. Wycieczka z przewodnikiem|
       75 50 |2011-05-14|)))))
(defun wycieczki-lokalizacja(w) (nth 0 w))
(defun wycieczki-pensjonat(w) (nth 1 w))
(defun wycieczki-pensjonat-adr(w) (nth 2 w))
(defun wycieczki-cena(w) (nth 3 w))
(defun wycieczki-miejsc(w) (nth 4 w))
(defun wycieczki-datawy(w) (nth 5 w))
(defun wycieczki-dataprzy(w) (nth 6 w))
(defun wycieczki-dodatki(w) (nth 7 w))
(defun dodatki-lokalizacja(d) (nth 0 d))
(defun dodatki-opis(d) (nth 1 d))
(defun dodatki-cena(d) (nth 2 d))
(defun dodatki-miejsc(d) (nth 3 d))
(defun dodatki-data(d) (nth 4 d))

(defun wycieczki-check-integrity()
  (let ((bad nil))
    (dolist (w wycieczki bad)
      (when (or (/= (length w) 8)
		(and (not (null (nth 7 w)))
		     (not (consp (nth 7 w)))
		     (every #'(lambda(p) (or (/= (length p) 5)
					     (not (numberp (nth 2 p)))
					     (not (numberp (nth 3 p)))))
			    (nth 7 w)))
		(not (numberp (nth 3 w)))
		(not (numberp (nth 4 w))))
	(push w bad)))))

(defparameter *Wycieczki-err* (wycieczki-check-integrity))

(defun gen-Wycieczki-prepare ()
  (when (null *Wycieczki-err*)
    (setf wycieczki (remove-duplicates wycieczki))
    (permutate-list! wycieczki)))

(defun gen-Wycieczki (n)
  (labels ((gen-K ()
	     (let ((res nil))
	       (dolist (k (remove-duplicates (mapcar #'wycieczki-lokalizacja wycieczki)) res)
		 (push (concatenate 'string
				    "select '"
				    (symbol-name k)
				    "'")
		       res))))
	   (gen-W ()
	     (let ((w (car wycieczki)))
	       (setf wycieczki (cdr wycieczki))
	       (concatenate 'string
			    "select (select ID_kraju from Kraje where cast(Nazwa_kraju as varchar(30)) = '"
			    (symbol-name (wycieczki-lokalizacja w))
			    "'), '"
			    (symbol-name (wycieczki-pensjonat w))
			    "', '"
			    (symbol-name (wycieczki-pensjonat-adr w))
			    "', "
			    (format nil "~D" (wycieczki-cena w))
			    ", "
			    (format nil "~D" (wycieczki-miejsc w))
			    ", '"
			    (symbol-name (wycieczki-datawy w))
			    "', '"
			    (symbol-name (wycieczki-dataprzy w))
			    "'"))))
    (when (plusp n)
      (concatenate 'string
		   (format nil "~%~%insert into Kraje (~A) ~{~&~2t~A~^~&union all~}~%"
			   *Kraje-table*
			   (gen-K))
		   (format nil "~%~%insert into Wycieczki (~A) ~{~&~2t~A~^~&union all~}~%"
			   *Wycieczki-table*
			   (loop for i from 1 to n when (not (null wycieczki)) collect (gen-W)))))))