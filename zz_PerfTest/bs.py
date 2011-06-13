#!/usr/bin/env python

from timeit import Timer
import random

def binsearch(K, vect, size):
	a = 0
	size = size - 1
	while size >= a:
		i = (a+size)/2
		val = vect[i]
		if K >= val:
			if K == val: return i
			a = i + 1
		else:
			size = i - 1
	return -1

A = range(32)

def onetest():
	max = 1000
	j = (int)(32*random.random())
	for k in range(max):
		r = binsearch(j, A, 32)

def testit():
	t = Timer("onetest()", "from bs import binsearch,onetest,A")
	print t.repeat(1,1000)

if __name__ == "__main__":
	print "TEST:"
	testit()

