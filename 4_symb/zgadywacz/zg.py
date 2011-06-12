#!/usr/bin/env python
# -*- coding: utf-8

def isArithm(lst):
    """ Sprawdza, czy podany ciąg jest arytmetyczny.
    Zwraca różnicę ciągu. """
    if len(lst) <= 2: return lst and lst[0] or False
    d = lst[1] - lst[0]
    p = lst[0] - d
    for x in lst:
        if x-p != d: return False
        d = x-p
        p = x
    return d
def nextArithm(lst,d):
    """ Zwraca następny element lst -- ciągu arytmetycznego o różnicy r. """
    return lst[-1]+d
def isGeom(lst):
    """ Sprawdza, czy podany ciąg jest geometryczny.
    Zwraca iloraz ciągu. """
    if len(lst) <= 2: return lst and lst[0] or False
    zrs = [i for i,x in enumerate(lst) if x == 0]
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
    """ Zwraca następny element lst -- ciągu geometrycznego o ilorazie r. """
    return lst[-1]*r
def diffSub(lst):
    """ Zwraca listę pochodnych ciągu lst. """
    res = []
    p = lst[0]
    for x in lst[1:]:
        res.append(x-p)
        p = x
    return res
def diffDiv(lst):
    """ Zwraca listę ilorazów par ciągu lst lub False gdy lista zawiera zero. """
    if [i for i in lst if i==0]: return False
    res = []
    p = lst[0]
    for x in lst[1:]:
        res.append(x/p)
        p = x
    return res

testers = (isArithm, isGeom)
resulters = (nextArithm, nextGeom)
differs = (diffSub, diffDiv)

def dfs(lst,lvl=7):
    """ Zgaduje następny element ciągu lst. """
    if lvl < 1 or not lst: return False
    for i,f in enumerate(testers):
        res = f(lst)
        if res: return resulters[i](lst,res)
    for i,d in enumerate(differs):
        res = dfs(d(lst), lvl-1)
        if res: return resulters[i](lst,res)
    return False

if __name__ == '__main__':
    print dfs(input())
