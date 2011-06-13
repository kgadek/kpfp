#from multiprocessing import Pool
import multiprocessing
import time

def f(x):
    return x*x

def multimap(fun, lst):
    return multiprocessing.Pool(processes=2).map(fun, lst)

def simplemap(fun, lst):
    return map(fun,lst)


def multireduce(fun, lst, zero=0):
    def myreduce(blah):
        print "JOJOJO"
        return 2 #len(blah)==2 and fun(blah[0], blah[1]) or zero
    div = len(lst)/2
    print "JO"
    pool = multiprocessing.Pool(processes=2)
    res1 = pool.apply_async(sum, [lst[:div]])
    res2 = pool.apply_async(sum, [lst[div:]])
    res1.wait()
    res2.wait()
    return fun(res1.get(), res2.get())
    

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
