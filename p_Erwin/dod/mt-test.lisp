;;; To run all the test programs, evaluate "(check *tests*)".
;;; You can also run the test programs individually, like this:
;;; "(test0001)".

(load "mt.lisp")
(in-package "MERSENNE-TWISTER")

(defvar *tests* nil)
(setq *tests* nil)

(defmacro deftest (name &rest args-doc-body)
  `(progn
     (setq *tests* (append *tests* (list ',name)))
     (defun ,name ,@args-doc-body)))

(defun run-test (test-fn)
  (format t "~&~A" test-fn)
  (let ((rc (funcall test-fn)))
    (unless rc
      (format t "~&*** failed ***"))
    rc))

(defun check (lst)
  "Executes all the tests in LST until they all execute or one of
them fails."
  (do ((x lst (rest x)))
      ((or (endp x)
	   (not (run-test (first x)))))))

(deftest test0000 ()
  "Null test.  Always succeeds."
  t)

(deftest test0001 ()
  "Test that we can call MT:MT-GENRAND without crashing."
  (mt:mt-genrand)
  t)

(deftest test0002 ()
  "Test that we can call MT:MT-RANDOM with N = 10 without crashing."
  (mt:mt-random 10)
  t)

(deftest test0003 ()
  "Test that we can call MT:MT-RANDOM with N = 1.0 without crashing."
  (mt:mt-random 1.0)
  t)

(deftest test0004 ()
  "Test that MT:MT-RANDOM always returns 0 <= R <= LIMIT for a LIMIT
of 10 & where 'always' is sort of a statistical thing: We actually
call MT:MT-RANDOM just 10,000 times."
  (do ((count 10000)
       (limit 10)
       (i 0 (1+ i))
       (r 0 (mt:mt-random limit)))
      ((or (>= i count) (< r 0) (< limit r))
       (cond ((and (<= 0 r) (< r limit))
	      t) ; good
	     (t
	      (format t "~&error:")
	      (format t " ~A returned ~D," 'mt:mt-random r)
	      (if (< r 0)
		  (format t " which is less than zero & should not be.")
		(format t " which is ~A ~D & should be less."
			(if (= r limit) "equal to" "greater than")
			limit))
	      (format t "~&(~A is ~A.)" 'i i)
	      nil)))))

(deftest test0005 ()
  "Test that we can call MT:MT-RANDOM with a limit that's a bignum without
crashing."
  (mt:mt-random (1+ (expt 2 65)))
  t)

(deftest test0006 ()
  "Like TEST0004 but uses a bignum for the limit."
  (do ((count 10000)
       (limit (1+ (expt 2 65)))
       (i 0 (1+ i))
       (r 0 (mt:mt-random limit)))
      ((or (>= i count) (< r 0) (< limit r))
       (cond ((and (<= 0 r) (< r limit))
	      t) ; good
	     (t
	      (format t "~&error:")
	      (format t " ~A returned ~D," 'mt:mt-random r)
	      (if (< r 0)
		  (format t " which is less than zero & should not be.")
		(format t " which is ~A ~D & should be less."
			(if (= r limit) "equal to" "greater than")
			limit))
	      (format t "~&(~A is ~A.)" 'i i)
	      nil)))))

(deftest test0007 ()
  "Test that MT:MT-RANDOM doesn't crash when the limit is a floating point
number."
  (mt:mt-random 17.5)
  t)

(deftest test0008 ()
  "Like TEST0004 but the limit is a floating point number."
  (do ((count 10000)
       (limit 17.0)
       (i 0 (1+ i))
       (r 0 (mt:mt-random limit)))
      ((or (>= i count) (< r 0) (< limit r))
       (cond ((and (<= 0 r) (< r limit))
	      t) ; good
	     (t
	      (format t "~&error:")
	      (format t " ~A returned ~D," 'mt:mt-random r)
	      (if (< r 0)
		  (format t " which is less than zero & should not be.")
		(format t " which is ~A ~D & should be less."
			(if (= r limit) "equal to" "greater than")
			limit))
	      (format t "~&(~A is ~A.)" 'i i)
	      nil)))))

;;; --- end of file ---
