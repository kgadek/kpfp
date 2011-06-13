#from multiprocessing import Pool
import multiprocessing
import time

def f(x):
    return x*x

def multimap(fun, lst):
    return multiprocessing.Pool(processes=2).map(fun, lst)

def simplemap(fun, lst):
    return map(fun,lst)

__verytopsecret = sum
def __notsosecret(x,y):
    #return reduce(__verytopsecret, x)
    return apply(__verytopsecret, ([x,y],))
def multireduce(fun, lst, zero=0):
    div = len(lst)/2
    __verytopsecret = fun
    pool = multiprocessing.Pool(processes=2)
    res1 = pool.apply_async(__notsosecret, (10, 20, ))
    res2 = pool.apply_async(__notsosecret, (10, 20, ))
    res1.wait()
    res2.wait()
    return __notsosecret(res1.get(), res2.get())
    

if __name__ == '__main__':
    t0=time.clock()
    r0 = multimap(f, range(100000))
    t1=time.clock()
    r1 = simplemap(f, range(100000))
    t2=time.clock()
    print t1-t0
    print t2-t1
    print r0==r1
    print "Res=%s" % multireduce(lambda x,y: x+y, range(5))
