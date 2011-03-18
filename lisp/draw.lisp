#|
Konwertuje plik z opisem drzewa współpracowników FM Group[1] (plik *.csv; patrz: +datafile+)
do języka dot[2].

Linki:
[1] http://www.perfumy.fm/inc/tree2.php
[2] http://www.graphviz.org/
|#

(declaim (optimize (speed 3) (compilation-speed 0) (safety 0) (debug 0)))

;;; Parametry programu
(defconstant +drzewo-filename+ "drzewko aktualne.csv")      ; Stała wskazująca plik *.csv z danymi
(defconstant +startup-time+ 14)              ; Czas na zapoznanie się z biznesem
(defconstant +newbie-colour+ "\"\#41924B\"")
(defconstant +nonactive-colour+ "\"\#FF3333\"")
(defconstant  +3-COLOUR+ "\"\#D8D8C0\"")    ; Kolory poziomów:
(defconstant  +6-COLOUR+ "\"\#A9AD98\"")    ;   http://www.colourlovers.com/palette/244473/zen_persimmon
(defconstant  +9-COLOUR+ "\"\#7A8370\"")    ; Po dodaniu kolorów przejściowych:
(defconstant +12-COLOUR+ "\"\#AC8442\"")    ;   http://tinyurl.com/3676gks
(defconstant +15-COLOUR+ "\"\#DF8615\"")
(defconstant +18-COLOUR+ "\"\#EB660A\"")
(defconstant +21-COLOUR+ "\"\#F84600\"")
(defconstant month-sums (apply #'vector (nreverse (maplist #'(lambda(x) (apply #'+ x)) (nreverse '(0 31 28 31 30 31 30 31 31 30 31 30 31))))))
(defconstant yzero 2000)

;;; Zmienne globalne
(defvar curr-d 0)
(defvar curr-m 0)
(defvar curr-y 0)
(defvar root nil)
(defvar curr nil)

;;; Użyte makra
(defmacro only-if (con &body body) `(if ,con (progn ,@body)))
(defmacro while (con &body body) `(do () ((not ,con)) ,@body))
(defmacro strconcat (str x) `(setf ,str (concatenate 'string ,str ,x)))
(defun string->var (n) (intern (format nil "+~D-COLOUR+" n)))
(defmacro colour-by-obrgr (lst)
  (let ((res nil))
    (push '(t "") res)
    (dolist (i lst) (push `((> (node-obrgr n) ,@(cdr i)) (format nil "color = ~A~%    style = filled~%" ,(string->var (car i)))) res))
    (push `((node-nieakt n) (format nil "color = ~A~%    penwidth = 2~%    " +newbie-colour+)) res)
    (push `((and (node-nieakt n) (> (node-days n) +startup-time+)) (format nil "color = ~A~%    penwidth = 2~%    "+nonactive-colour+)) res)
    (push 'cond res) res))

;;; Obliczanie ilości dni od wejścia danej osoby do struktury.
(defun leap? (y)
    (and (zerop (mod y 4))
       (or (zerop (mod y 400))
         (not (zerop (mod y 100))))))
(defun month->num (m y)
  (+ (svref month-sums (- m 1))
     (if (and (> m 2) (leap? y)) 1 0)))
(defun year-days (y)
    (if (leap? y) 366 365))
(defun year->num (y)
    (let ((d 0))
      (if (>= y yzero)
        (dotimes (i (- y yzero) d)
          (incf d (year-days (+ yzero i))))
        (dotimes (i (- yzero y) (- d))
          (incf d (year-days (+ y i)))))))
(defun date->num (d m y)
  (+ (- d 1) (month->num m y) (year->num y) (- 21)))
(defun days-since (d1 m1 y1 d2 m2 y2)
  (- (date->num d2 m2 y2) (date->num d1 m1 y1)))

;;; Konwersja stringa do liczby
(defun num (x) (read-from-string x :junk-allowed t))

;;; Struktura przechowująca informacje o drzewie
(defstruct (node
    ;; Funkcja wyświetlająca
    (:print-function (lambda (n stream depth)
      (declare (ignore depth))
      (format stream
"\"~A\" [
    ~Alabel = \"{~A~A ~A ~A | ~A ~A | ~,2F (+~,2F) b:~,2F | ~D% ~,2Fzl (~,2Fzl) b:~,2F}\"~%];~%
\"~A\" -> \"~A\" [ label = \"~D:(~D)~A\"]~%;"
        (node-id n)
        (colour-by-obrgr ((3 300)(6 1200)(9 3600)(12 7200)(15 12000)(18 20400)(21 30000)))
        (node-imie n)
        (if (node-nieakt n) (format nil " (~D dni)" (node-days n)) "")
        (if (node-mail n) (format nil " | ~A " (node-mail n)) "")
        (if (node-tel n) (format nil "| ~A" (node-tel n)) "")
        (node-id n)
        (subseq (format nil "~A" (node-wst n)) 2)
        (node-obrgr n)
        (node-obrwl n)
        (node-bal n)
        (truncate (node-pro n))
        (if (> (node-obrwl n) 34.42) (node-zar n) 0.00)
        (if (> (node-obrwl n) 34.42) (node-zarwl n) 0.00)
        (node-balobr n)
        (node-parid n)
        (node-id n)
        (node-lvl n)
        (1+ (node-childrentotal n))
        (if (or (eql (node-par n) nil) (equalp (node-parzar n) 0.00) (< (node-obrwl (node-par n)) 43.42))
          ""
          (format nil "\\n~,2Fzl"(node-parzar n)))))))

  ;; Zawartość struktury
  (id "")          ; ID w systemie
  (imie "")        ; Imię i nazwisko
  (mail "")        ; Podany adres email
  (tel nil)        ; Podany nr telefonu
  (obrwl 0.00)    ; Obrót własny
  (obrgr 0.00)    ; Obrót grupy (suma)
  (pro 0.00)      ; Osiągnięty próg
  (bal 0.0)        ; Współczynnik zrównoważenia drzewa
  (balobr 1.0)    ; Współczynnik zrównoważenia obrotów
  (zar 0.00)      ; Zarobek
  (zarwl 0.00)    ; Zarobek osobisty
  (parzar 0.00)    ; Zarobek osoby wyżej z danej grupy
  (wst "2010-01-01")  ; Data wstąpienia
  (nieakt nil)    ; Miesięcy nieaktywny
  (days 0)        ; Dni w strukturze
  (children nil)  ; Osoby niżej w strukturze.
  (childrentotal 0)    ; Całkowita ilość osób niżej
  (par nil)        ; Osoba wyżej w strukturze.
  (parid "")      ; ID osoby wyżej w strukturze
  (lvl 0))

;;; Zamiana polskich znaków na miedzynarodowe odpowiedniki (zażółć gęślą jaźń -> zazolc gesla jazn),
;;; gdyz powoduja problemy z graphviz.
(defun unpolish (mystr)
  (dolist (chg '((#\ż #\z) (#\ó #\o) (#\ł #\l) (#\ć #\c) (#\ę #\e) (#\ś #\s) (#\ą #\a) (#\ź #\z) (#\ń #\n)
           (#\Ż #\Z) (#\Ó #\O) (#\Ł #\L) (#\Ć #\C) (#\Ę #\E) (#\Ś #\S) (#\Ą #\A) (#\Ź #\Z) (#\Ń #\N)))
    (setf mystr (substitute (second chg) (first chg) mystr)))
  mystr)

;;; Zamiana znaku @ na \@ w adresie mailowym
(defun unmail (x)
  (let ((p (position #\@ x)))
    (if p (format nil "~A\@~A"
        (subseq x 0 p)
        (subseq x (+ p 1))))))

;;; Funkcje dzielenia napisu na tokeny.
(defun constituent (c) (lambda (x) (and (graphic-char-p x) (not (char= x c)))))
(defun tokens (str test &key (start 0))
  (let ((p1 (position-if test str :start start)))
    (if p1 (let ((p2 (position-if #'(lambda (c) (not (funcall test c))) str :start p1)))
              (cons (subseq str p1 p2) (if p2 (tokens str test :start p2) nil)))
            nil)))

;;; Dodawanie osoby do drzewa
(defun addn (new obj)
  (cond ((not obj)
          (setf root new) (setf curr root))
        ((not (string= (node-id obj) (node-parid new)))
          (addn new (node-par obj)))
        (t (setf (node-par new) obj)
          (push new (node-children obj))
          (setf curr new))))

;;; Wyznaczanie współczynnika balansu
(defun calc-bal (lst)
  "Wyznaczanie współczynnika balansu. Autor wzoru: Radek Łazarz"
  (let* ((len (length lst))
         (sum (apply #'+ lst))
         (A (/ sum len))
         (S (sqrt (- (/ (apply #'+ (mapcar #'(lambda (x) (* x x)) lst)) len) (* A A)))))
    (- 1 (/ (atan (expt (/ S A) (log sum))) (/ PI 2)))))

;;; DFS
(defun dfs (x)
  (only-if x
      (if (node-par x) (setf (node-parzar x) (* 0.01 (node-obrgr x) (- (node-pro (node-par x)) (node-pro x)))))
      (setf (node-zarwl x) (* 0.01 (node-pro x) (node-obrwl x))
            (node-zar x) (node-zarwl x)
            (node-childrentotal x) 0)
      (let ((chldnumlst nil) (chldobrlst nil))
        (dolist (ch (node-children x))
          (setf (node-lvl ch) (1+ (node-lvl x)))
          (multiple-value-bind (chldzar chldtot chldobr) (dfs ch)
            (incf (node-zar x) chldzar)
            (incf (node-childrentotal x) chldtot)
            (push chldtot chldnumlst)
            (push chldobr chldobrlst)))
        (if (> (length chldnumlst) 1) (setf (node-bal x) (calc-bal chldnumlst)))
        (if (> (apply #'+ chldobrlst) 0) (setf (node-balobr x) (calc-bal chldobrlst))))
      (values (node-parzar x) (+ 1 (node-childrentotal x)) (node-obrgr x))))

;;; Wyświetlanie nagłówka pliku dot
(defun print-header (data godzina)
  (let ((dt (tokens data (constituent #\-))))
    (setf curr-y (num (first dt)) curr-m (num (second dt)) curr-d (num (third dt)))
    (format t "digraph \"~A ~A\" {~%node [~%    rankdir = LR~%    shape = Mrecord~%    fontsize = 16~%];~%  edge [~%    fontsize = 20~%];~2%"
      data godzina)))

;;; Wyświetlanie drzewa
(defun print-tree (n)
  (only-if n
    (print n)
    (dolist (ch (node-children n)) (print-tree ch))))

;;; Wyszukiwanie wzorca (algorytm KMP)
(defun pref-suf (str len)
  (do* ((j -1)
        (p (make-array len :initial-element -1))
        (i 1 (+ i 1)))
      ((<= len i) p)
      (while (and (>= j 0) (not (char= (char str i) (char str (+ j 1)))))
        (setf j (svref p j)))
      (if (equalp (char str i) (char str (+ j 1)))
        (setf j (+ j 1)))
      (setf (svref p i) j)))
(defun kmp (str wz)
  (do* ((i 0 (+ i 1))
        (j 0)
        (strlen (length str))
        (wzlen (length wz))
        (p (pref-suf wz wzlen))
        (res nil))
      ((<= strlen i) (nreverse res))
      (while (and (>= j 0) (not (char= (char str i) (char wz (+ j 1)))))
        (setf j (svref p j)))
      (if (char= (char str i) (char wz (+ j 1)))
        (setf j (+ j 1)))
      (only-if (equalp (+ j 1) wzlen)
          (push (- i j) res)
          (setf j (svref p j)))))

;;; Wyciąganie numeru telefonu ze stringa
(defun get-telnum (str)
  (let ((out nil))
    (dolist (res (kmp str "+48-"))
      (strconcat out "|")
      (strconcat out (subseq str res
          (position-if #'(lambda(x) (not (or (digit-char-p x) (char= x #\-))))
                str :start (+ 1 res)))))
    (if out (subseq out 1))))

;;; Główna funkcja
(defun main (datafile)
  (with-open-file (infile datafile)
    (let ((dane_we_data (tokens (read-line infile nil) (constituent #\ ))))
      (print-header (fourth dane_we_data) (subseq (fifth dane_we_data) 0 8)))
    (read-line infile nil) ; Pomija linię opisową
    (do  ((in (read-line infile nil) (read-line infile nil))
         (dt '(1990 01 22)))
        ((null in))
        (setf in (tokens in (constituent #\;)))
        (setf dt (tokens (ninth in) (constituent #\-)))
        (addn (make-node
            :id (subseq (second in) 0 8)
            :imie (let ((desc (tokens (fourth in) (constituent #\ ))))
                    (format nil "~A ~A" (unpolish (first desc)) (unpolish (second desc))))
            :mail (unmail (first (last (tokens (fourth in) (constituent #\ )))))
            :obrwl (num (substitute #\. #\, (fifth in)))
            :obrgr (num (substitute #\. #\,  (sixth in)))
            :pro (num (seventh in))
            :wst (num (ninth in))
            :nieakt (string= (tenth in) "1")
            :parid (subseq (third in) 0 8)
            :days (days-since (num (third dt)) (num (second dt)) (num (first dt)) curr-d curr-m curr-y)
            :tel (get-telnum (fourth in)))
          curr))
    (dfs root)
    (print-tree root)
    (format t "~2%}~%")))
(main +drzewo-filename+)

(defun profiles ()
  (sb-profile:profile  main leap? month->num year-days year->num date->num days-since unpolish unmail constituent tokens addn dfs print-header pref-suf kmp get-telnum main a sqr sigma calc-bal))
(defun watch ()
  (sb-profile:unprofile)
  (sb-profile:reset)
  (profiles)
  (main +drzewo-filename+)
  (sb-profile:report)
  (sb-profile:unprofile))

