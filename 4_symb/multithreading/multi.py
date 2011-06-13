#from multiprocessing import Pool
import multiprocessing
import time

def f(x):
    return x*x

def multi(fun, lst):
    return multiprocessing.Pool(processes=2).map(fun, lst)

def niemulti(fun, lst):
    return map(fun,lst)

if __name__ == '__main__':
    t0=time.clock()
    multi(f, range(1000000))
    t1=time.clock()
    niemulti(f, range(1000000))
    t2=time.clock()
    print t1-t0
    print t2-t1
