#!/usr/bin/env python
# -*- coding: utf-8

def oprank(op):
    if op in ['(', ')']: return 3
    elif op in ['*',]: return 7
    elif op in ['+', '-']: return 5
    
def pprint(expr,indent=0):
    if type(expr) == type([]):
        return '(%s)' % (' '.join(map(lambda x: pprint(x, indent+2), expr)),)
    return '%s' % (expr,)

operators =['+', '-', '*', '^']

def operatorrank(op):
    if op == '+': return 5
    if op == '-': return 5
    if op == '*': return 7
    if op == '^': return 9

def gettok(strin):
    return strin[0]

def arity(operator):
    return 2

def mkexpr(op, args): 
    return '(%s %s)' % (op, ' '.join(map(lambda x: str(x), args)), )

def parse(strin):
    op = [  ]
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
    while len(op) > 0:
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
            '2+3*4', '2*3+4', '(2+3)*4', '2+(3*4)', '(2*3)+4', '2*(3+4)'
            '(a+b+c)*(d-e)*(1-f)*g*6^2*2'
            ]
    for strin in strs:
        print strin
        print parse(strin)[0]
        print '===================================================================================================='
    