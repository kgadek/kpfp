(in-package :org.gadek.HansGen)

(defconstant +Klienci-pr-firma+ 0.1)
(defconstant +Klienci-pr-firmaNazwa+ 0.5)
(defconstant +Klienci-pr-indywNip+ 0.2)
(defparameter *Klienci-table* "Nazwa, Rodzaj_klienta, Adres, Miasto, Kod_pocztowy, Telefon_kontaktowy, NIP")

(defparameter imiona
  '(|Aleksander| |Bartłomiej| |Grzegorz| |Aleksandra| |Alicja| |Maciej| |Tomasz| |Konrad| |Michał|
    |Jakub| |Marcin| |Mirosław| |Piotr| |Aneta| |Igor| |Maksymilian| |Łukasz| |Wojtek| |Elżbieta|
    |Irek| |Krzysztof| |Agnieszka| |Beata| |Damian| |Magda| |Malwina| |Monika| |Marysia| |Natalia|
    |Wiola| |Paulina| |Józef| |Mateusz| |Kinga| |Radosław| |Adam| |Adrian| |Adolf| |Albin|
    |Ada| |Adela| |Agata| |Agnieszka| |Aldona| |Edward| |Jacek|))

(defparameter nazwiska
  '(|Krasowski| |Fuk| |Dulian| |Korzycki| |Bubak| |Valenta| |Schaefer| |Dyduch| |Topa| |Nikolow|
    |Długopolski| |Bieniasz| |Paszyński| |Zaborowski| |Żabiński| |Marcjan|
    |Nowak| |Kowalski| |Wiśniewski| |Dąbrowski| |Lewandowski| |Wójcik| |Kamiński| |Kowalczyk| |Zieliński|
    |Szymański| |Woźniak| |Kozłowski| |Jankowski| |Wojciechowski| |Kwiatkowski| |Kaczmarek| |Mazur|
    |Krawczyk| |Piotrowski| |Grabowski| |Nowakowski| |Pawłowski| |Michalski| |Nowicki| |Adamczyk| |Dudek|
    |Zając| |Wieczorek| |Jabłoński| |Król| |Majewski| |Wojewódzki| |Olszewski|
    |Kusak| |Jurkowski| |Anielski| |Gądek| |Dylong| |Ozga| |Żaba| |Madziar| |Wesół| |Rudol| |Witek|
    |Strojniak| |Zębalski| |Dykacz| |Sadecki| |Gankiewicz| |Godzik| |Łazarz| |Molenda| |Zubel| |Drabek|
    |Berezowski| |Niziołek| |Zbrzeżny| |Chłosta| |Brzęcki| |Zachariasiewicz| |Rakoczy| |Marczak|
    |Cetnarowicz| |Pałosz| |Przybyło| |Adamus| |Żak| |Niedoba| |Chwiej| |Kułakowski| |Gajęcki|
    |Boryczko| |Wantuch| |Wasiewicz-Porębski| |Boroń| |Golański| |Domański| |Wojda| |Paszkowska| |Chyla|
    |Faliszewski| |Pisarek| |Jurczyk| |Pietras| |Chłosta|))

(defparameter firmy-pre
  '(|Pol| |Krak| |Stal| |Mix| |Gaz| |Ex-| |Mex-| |Kol| |Ser| |Wars|))
(defparameter firmy-suf
  '(|tur| |export| |dex| |soft| |dent| |farm| |med| |comp|))

(defparameter miasta ; > 100,000 oraz Zawiercie na specjalne życzenie Radka
  '(|Kraków| |Warszawa| |Poznań| |Gniezno| |Zadupie| |Katowice| |Rzeszów| |Gdańsk| |Gdynia|
    |Łódź| |Wrocław| |Sopot| |Szczecin| |Bydgoszcz| |Lublin| |Białystok| |Częstochowa|
    |Radom| |Sosnowiec| |Toruń| |Kielce| |Gliwice| |Zabrze| |Bytom| |Rzeszów| |Olsztyn|
    |Bielsko-Biała| |Ruda Śląska| |Rybnik| |Tychy| |Dąbrowa Górnicza| |Płock| |Elbląg|
    |Opole| |Gorzów Wielkopolski| |Wałbrzych| |Zielona Góra| |Włocławek| |Tarnów| |Chorzów|
    |Koszalin| |Kalisz| |Legnica| |Zawiercie|))

(defparameter ulice
  '(|ul. Czarnowiejska| |ul. Nawojki| |ul. Reymonta| |al. Adama Mickiewicza| |ul. Królewska|
    |ul. Bronowicka| |ul. Armii Krajowej| |ul. Piastowska| |al. Marszałka Ferdinanda Focha|
    |ul. Podchorążych| |ul. Królewska| |al. Kijowska| |al. Juliusza Słowackiego|
    |ul. Królowej Jadwigi| |ul. Marii Konopnickiej| |ul. Karmelicka| |ul. Tadeusza Kościuszki|
    |ul. Monte Cassino| |ul. Księcia Józefa| |ul. Generała Bohdana Zielińskiego|
    |ul. Zwierzyniecka| |ul. Długa| |ul. Prądnicka| |ul. Wrocławska| |ul. Doktora Twardego|
    |ul. Józefa Wybickiego| |ul. Warszawska| |ul. Pawia| |ul. Wita Stwosza| |al. 29 Listopada|
    |ul. Opolska| |ul. Josepha Conrada| |ul. Friedleina| |ul. Nad Sudołem| |ul. Sobieskiego|
    |ul. Kluczborska| |al. Pokoju| |ul. Kawiory| |ul. Miechowska| |ul. Toruńska| |ul. Witolda Budryka|
    |ul. Józefa Rostafińskiego|))

(let ((nip-wsp '(6 5 7 2 3 4 5 6 7)))
  (defun nip-check (nip)
    (if (or (not (numberp nip))
	    (< nip 1000000000)
	    (> nip 9999999999))
	nil
	(let ((checksum (mod (reduce #'+
				     (mapcar #'*
					     (do ((res nil)
						  (np (floor nip 10) (floor np 10)))
						 ((zerop np) res)
					       (push (mod np 10) res))
					     nip-wsp))
			     11)))
	  (= checksum (mod nip 10))))))

(defun gen-postcode ()
  (format nil "~2,'0D-~3,'0D" (random 100) (random 1000)))

(defun gen-nip ()
  (do ((nip 0 (+ 1000000000 (random 9000000000))))
      ((nip-check nip) nip)))

(defun gen-phone ()
  (let* ((space (when (flip) "-"))
	 (prefix (when (flip) (concatenate 'string
					   "+48"
					   space))))
    (concatenate 'string
		 prefix
		 (format nil "~3,'0D" (random 1000))
		 space
		 (format nil "~3,'0D" (random 1000))
		 space
		 (format nil "~3,'0D" (random 1000)))))

(defun gen-address ()
  (concatenate 'string
	       (symbol-name (choose-rnd ulice))
	       (format nil " ~D~A" (random 100) (if (flip 0.25) (choose-rnd '(a b c)) ""))))

(let ((name-pairs nil))
  (defun gen-name ()
    (when (not (null name-pairs))
      (let ((ret (car name-pairs)))
	(setf name-pairs (cdr name-pairs))
	ret)))
  (defun gen-name-prepare ()
    (setf name-pairs nil)
    (dolist (a imiona)
      (dolist (b nazwiska)
	(push (format nil "~A ~A" (symbol-name a) (symbol-name b)) name-pairs)))
    (permutate-list! name-pairs)
    (length name-pairs)))

(let ((comp-pairs nil))
  (defun gen-firma ()
    (when (not (null comp-pairs))
      (let ((ret (car comp-pairs)))
	(setf comp-pairs (cdr comp-pairs))
	ret)))
  (defun gen-firma-ready ()
    (not (null comp-pairs)))
  (defun gen-firma-prepare ()
    (setf comp-pairs nil)
    (dolist (a firmy-pre)
      (dolist (b firmy-suf)
	(push (format nil "~A~A" (symbol-name a) (symbol-name b)) comp-pairs)))
    (permutate-list! comp-pairs)
    (length comp-pairs)))

(defun gen-Klienci-prepare ()
  (permutate-list! imiona)
  (setf nazwiska (remove-duplicates nazwiska))
  (permutate-list! nazwiska)
  (setf firmy-pre (remove-duplicates firmy-pre))
  (permutate-list! firmy-pre)
  (setf firmy-suf (remove-duplicates firmy-suf))
  (permutate-list! firmy-suf)
  (setf miasta (remove-duplicates miasta))
  (permutate-list! miasta)
  (setf ulice (remove-duplicates ulice))
  (permutate-list! ulice)
  (gen-firma-prepare)
  (gen-name-prepare)
  (gen-firma-prepare))

(defun gen-Klienci (n)
  (labels ((gen-K ()
	     (let ((czyFirma (flip +Klienci-pr-firma+)))
	       (concatenate 'string
			    "select '"
			    (if (and czyFirma (flip +Klienci-pr-firmaNazwa+) (gen-firma-ready))
				(gen-firma)
				(gen-name))
			    "', '"
			    (format nil "~D" (if czyFirma "F" "O"))
			    "', '"
			    (gen-address)
			    "', '"
			    (symbol-name (choose-rnd miasta))
			    "', '"
			    (gen-postcode)
			    "', '"
			    (gen-phone)
			    "', "
			    (if (or czyFirma (flip +Klienci-pr-indywNip+))
				(format nil "'~D'" (gen-nip))
				"NULL")))))
    (when (plusp n)
      (format nil "~&insert into Klienci (~A) ~{~&~2t~A~^~&union all~}~%"
	      *Klienci-table*
	      (loop for i from 1 to n collect (gen-K))))))
