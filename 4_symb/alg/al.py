#!/usr/bin/env python
# -*- coding: utf-8

def oprank(op):
    if op in ['(', ')']: return 9
    elif op in ['*']: return 7
    elif op in ['+', '-']: return 5

def pprint(lst, indent=0):
    if type(lst) == type([]):
        cwidth = indent+1
        print '%s(' % (''.ljust(indent),)
        for i in lst:
            pprint(i,cwidth)
        print '%s)' % (''.ljust(indent),)
    else:
        print '%s%s' % (''.ljust(indent),lst)

def parse1(str):
    opstack = []
    exstack = []
    symstart = -1
    for i,c in enumerate(str):
        print '>> parse i=%d c=%c opstack=%s exstack=%s' % (i,c,opstack,exstack)
        if c in ['(','*','+',')','-']:
            if symstart >= 0:
                print 'symbol: %s <-- %d-%c..%d-%c' % (str[symstart:i],symstart,str[symstart],i,str[i])
                exstack.append(str[symstart:i])
                symstart = -1
            if c == ')':
                op = opstack.pop()
                while op != '(':
                    print 'nawias close: op=%s opstack=%s exstack=%s' % (op, opstack, exstack)
                    exb = exstack.pop()
                    exa = exstack.pop()
                    print 'nawias close: push %s' % [op,exa,exb]
                    exstack.append([op,exa,exb])
                    op = opstack.pop()
            elif len(opstack) > 0 and c != '(' and oprank(opstack[-1]) <= oprank(c):
                op = opstack.pop()
                exb = exstack.pop()
                exa = exstack.pop()
                print 'restack: %s' % ([op,exa,exb],)
                exstack.append([op,exa,exb])
            if c != ')':
                opstack.append(c)
        elif symstart < 0:
            symstart = i
        print '<< parse i=%d c=%c\n\topstack=%s\n\texstack=%s\n' % (i,c,opstack,exstack)
    if symstart >= 0:
        exstack.append(str[symstart:i])
    while len(opstack) > 0:
        op = opstack.pop()
        exb = exstack.pop()
        exa = exstack.pop()
        exstack.append([op,exa,exb])
    print 'OPSTACK = %s' % (opstack,)
    print 'EXSTACK = %s' % (exstack,)
    return exstack

if __name__ == '__main__':
    pprint(parse1('(a+b+c)*(d-e)*(1-f)*gg*6666'))
    pass