#from multiprocessing import Pool
import multiprocessing
import time

def f(x):
    return x*x

def multireduce(fun, lst, zero=0):
    def inner(lst):
        if lst:
            if len(lst)==1: return lst[0]
            if len(lst)==2: return fun(lst[0],lst[1])
            div = len(lst)/2
            return multimap(inner, [lst[div:], lst[:div]])
        else: return zero
    return inner(lst)

def multimap(fun, lst):
    return multiprocessing.Pool(processes=2).map(fun, lst)

def simplemap(fun, lst):
    return map(fun,lst)
    

if __name__ == '__main__':
    t0=time.clock()
    r0 = multimap(f, range(100000))
    t1=time.clock()
    r1 = simplemap(f, range(100000))
    t2=time.clock()
    print t1-t0
    print t2-t1
    print r0==r1
    #print "Res=%s" % reduce(lambda x,y: x+y, [1,2])
