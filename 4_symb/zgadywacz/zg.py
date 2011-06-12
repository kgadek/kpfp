#!/usr/bin/env python
# -*- coding: utf-8

def isArithm(lst):
    """ Sprawdza, czy podany ciąg jest arytmetyczny.
    Zwraca różnicę ciągu. """
    if len(lst) <= 2: return lst
    d = lst[1]-lst[0]
    p = lst[0]-d
    for x in lst:
        if x-p != d: return False
        d = x-p
        p = x
    return d

def nextArithm(lst,d):
    return lst[-1]+d

def posOf(lst,elem):
    return [i for i,x in enumerate(lst) if x == elem]

def isGeom(lst):
    """ Sprawdza, czy podany ciąg jest geometryczny.
    Zwraca iloraz ciągu. """
    if len(lst) <= 2: return lst
    zrs = posOf(lst,0)
    if zrs and not (zrs == range(len(lst)) or zrs == range(len(lst))[1:]):
        return False
    if zrs: return 0
    r = float(lst[1])/float(lst[0])
    p = lst[0]/r
    for x in lst:
        if float(x)/float(p) != r: return False
        r = float(x)/float(p)
        p = x
    return r

def nextGeom(lst,r):
    return lst[-1]*r

testers = (isArithm, isGeom)
resulters = (nextArithm, nextGeom)

if __name__ == '__main__':
    lst = input()
    for i,f in enumerate(testers):
        res = f(lst)
        if res:
            print i
            print f.__name__
            print "Next: "
            print resulters[i](lst,res)

