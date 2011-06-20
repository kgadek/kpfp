#!/usr/bin/env python
# -*- coding: utf-8

operators = ['+', '-', '*', '^', '_']

class NotImplemented: pass

def listp(x):
    """ Stwierdza, czy obiekt jest listą. """
    return type(x) == type([])

class ExpIter:
    """ Iterator klasy Exp. """
    def __init__(self,exprs):
        self.exprs = exprs
    def __iter__(self):
        return self
    def next(self):
        if len(self.exprs) == 0:
            raise StopIteration
        tmp = self.exprs[0]
        self.exprs = self.exprs[1:]
        return tmp 

class Exp:
    """ Klasa bazowa, przechowująca pojedynczy przetwarzany element. """
    def __init__(self, expr = 1.0, sign=1):
        self.sign = sign
        self.expr = expr
    def __iter__(self):
        return ExpIter(list(self.expr))
    def __repr__(self):
        return '%s%s' % (self.sign < 0 and '-' or '', self.expr)
    def __mul__(self,other):
        print 'Mul z exp'
        return Mul([self,other])
    def __add__(self,other):
        print 'Add z exp'
        return Sum([self,other])

def classname(x): return x.__class__.__name__

class Sum(Exp):
    def __init__(self,expr = 1.0,sign=1):
        Exp.__init__(self,expr,sign)
        self.expr = list(expr)
    def __iter__(self):
        return ExpIter(self.expr)
    def __repr__(self):
        return '(' + '+'.join(map(lambda x: str(x), self.expr)) + ')'
    def __mul__(self,other):
        """ Uproszczenie wyrażenia: (a+b+c)(d+e) --> ad + bd + cd + ae + be + ce.
        Wiemy, że (a+b+c+...) * XYZ gdzie XYZ jest (potomkiem?) Exp. """
        tmp = []
        for i in self:
            for j in other:             
                tmp.append(Mul([i,j]))
        return Sum(tmp)
    def __add__(self,other):
        if classname(other) == 'Sum':
            tmp = list(self.expr)
            tmp.extend(other.expr)
            return Sum(tmp)
        tmp = list(self.expr)
        tmp.append(other.expr)
        return Sum(tmp)

class Mul(Exp):
    def __init__(self,expr = 1.0,sign=1):
        Exp.__init__(self,expr,sign)
        self.expr = list(expr)
    def __iter__(self):
        return ExpIter(self.expr)
    def __repr__(self):
        return '*'.join(map(lambda x: str(x), self.expr))
    def __mul__(self,other):
        if isinstance(other,Mul):
            tmp = list(self.expr)
            tmp.extend(other.expr)
        else:
            tmp = list(self.expr)
            tmp.append(other)
            return Mul(tmp, self.sign * other.sign)
    def __add__(self,other):
        return Sum([self,other])

#a = Exp('2')
#b = Exp('3')
#c = Exp('4')
#d = Exp('5')
#e = Exp('6')

def operatorrank(op):
    """ Zwraca priorytet operatora/funkcji. """
    if op == '+': return 5
    if op == '-': return 5
    if op == '_': return 5
    if op == '*': return 7
    if op == '^': return 9
    return 1

def gettok(strin):
    """ Zwraca pojedynczy token z początku stringu strin. """
    return strin[0]

def arity(operator):
    """ Zwraca ilość argumentów, jakie przyjmuje funkcja. """
    return operator == '_' and 1 or 2

def mkexpr(op, args): 
    """ Tworzy wyrażenie zbudowane z operatora i jego argumentów. """
    #if op == '*': return Exp(args)
    if not isinstance(args[0],Exp):
        args[0] = Exp(args[0])
    if not isinstance(args[1],Exp):
        args[1] = Exp(args[1])
    if op == '*':
        return args[0]*args[1]
    if op == '-':
        return args[0]-args[1]
    if op == '+':
        return args[0]+args[1]
    
#    tmp = [op]
#    tmp.extend(args)
#    return tmp
    #return '(%s %s)' % (op, ' '.join(map(lambda x: str(x), args)), ) # zwracanie lisp-str

def preparse(strin):
    """ Zwraca dobrze określone wyrażenie infiksowe (bez dwuznaczności, np. operator "-"
    w wyrażeniu (-a) zostaje zamieniony na operator "_" .
    Kod generalnie jest wolny, ochydny i jeszcze bardziej ochydny. """
    if strin[0] == '-': strin = '_' + strin[1:] # OMG jakie brzydkie
    for i in range(len(strin))[1:]:
        if strin[i] != '-': continue
        opf = False
        for op in operators:
            if strin[:i].endswith(op): opf = op
        if opf: strin = strin[:i] + '_' + (len(strin)>i+1 and strin[i+1:] or '') # OMG jakie TO jest dopiero brzydkie...
    return strin

def parse(strin):
    """ Parsuje dobrze określone wyrażenie infiksowe i zwraca wyrażenie w postaci prefiksowej, uproszczonej. """
    op = []
    ex = []
    i = 0
    while i < len(strin):
        tk = gettok(strin[i:])
        if tk == '(': # start nawias
            op.append(tk)
        elif tk == ')': # koniec nawias
            opt = op.pop()
            while opt != '(':
                arty = arity(tk)
                args = ex[-arty:]
                ex = ex[:-arty]
                ex.append(mkexpr(opt,args))
                opt = op.pop()
        elif not tk in operators: # symbol
            ex.append(tk)
        else: # operator
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
    print op
    print ex
    return ex

if __name__ == '__main__':
    strs = ['2', '2+2', '(2+2)',
            #'(m+n)*(m-n)-(m^2-n^2)',
            '2-2+2', '2+2*2',
            '(2+3+4)*(5*6*7)',
            '(2+3+4)*(5+6+7)',
            '2+3*4', '2*3+4', '(2+3)*4', '2+(3*4)', '(2*3)+4', '2*(3+4)',
            #'(a+b+c)*(d-e)*(1-f)*g*6^2*9*_2'
            ]
    for strin in strs:
        print strin
        print parse(preparse(strin))
        print '===================================================================================================='
    