#!/usr/bin/env python
# -*- coding: utf-8

operators = ['+', '-', '*', '^', '_'] 

def operatorrank(op):
    """ Zwraca priorytet operatora/funkcji. """
    if op == '+': return 5
    if op == '-': return 5
    if op == '*': return 7
    if op == '^': return 9
    if op == '_': return 5

def gettok(strin):
    """ Zwraca pojedynczy token z początku stringu strin. """
    return strin[0]

def arity(operator):
    """ Zwraca ilość argumentów, jakie przyjmuje funkcja. """
    return operator == '_' and 1 or 2

def mkexpr(op, args): 
    """ Tworzy wyrażenie zbudowane z operatora i jego argumentów. """
    return '(%s %s)' % (op, ' '.join(map(lambda x: str(x), args)), )

def preparse(strin):
    """ Zwraca dobrze określone wyrażenie infiksowe (bez dwuznaczności, np. operator "-"
    w wyrażeniu (-a) zostaje zamieniony na operator "_" . """
    return strin

def parse(strin):
    """ Parsuje dobrze określone wyrażenie infiksowe i zwraca wyrażenie w postaci prefiksowej, uproszczonej. """
    op = []
    ex = []
    i = 0
    while i < len(strin):
        tk = gettok(strin[i:])
        #print 'token=%s' % tk
        #print ' pre op=%s' % op
        #print ' pre ex=%s' % ex
        if tk == '(': # start nawias
            #print ' 1'
            op.append(tk)
        elif tk == ')': # koniec nawias
            #print ' 2'
            opt = op.pop()
            while opt != '(':
                arty = arity(tk)
                args = ex[-arty:]
                ex = ex[:-arty]
                ex.append(mkexpr(opt,args))
                opt = op.pop()
        elif not tk in operators: # symbol
            #print ' 3'
            ex.append(tk)
        else: # operator
            #print ' 4'
            if len(op) > 0 and operatorrank(tk) <= operatorrank(op[-1]): # operator o mniejszym lub równym priorytecie
                #print ' 4a'
                opt = op.pop()
                arty = arity(opt)
                args = ex[-arty:]
                ex = ex[:-arty]
                ex.append(mkexpr(opt,args))
            op.append(tk)
        i += len(tk)
    while len(op) > 0: # zredukuj co jeszcze nie zostało zredukowane
        opt = op.pop()
        arty = arity(opt)
        args = ex[-arty:]
        ex = ex[:-arty]
        ex.append(mkexpr(opt,args))
    return ex

if __name__ == '__main__':
    strs = ['2', '2+2', '(2+2)',
            '(m+n)*(m-n)-(m^2-n^2)',
            '2-2+2', '2+2*2',
            '2+3*4', '2*3+4', '(2+3)*4', '2+(3*4)', '(2*3)+4', '2*(3+4)',
            '(a+b+c)*(d-e)*(1-f)*g*6^2*9*_2'
            ]
    for strin in strs:
        print strin
        print parse(preparse(strin))
        print '===================================================================================================='
    