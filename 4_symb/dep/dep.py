class depends_callonce:
    _called = {}
    
    def __init__(self, *rest):
        self.rest = rest
    def __call__(self, method):
        def mcwrap():
            if not method in depends_callonce._called:
                for x in self.rest:
                    if not x in depends_callonce._called:
                        x()
                        depends_callonce._called[x] = True
                method()
                depends_callonce._called[method] = True
        return mcwrap

class depends:
    def __init__(self, *rest):
        self.rest = rest
    def __call__(self, method):
        def mcwrap():
            for x in self.rest:
                x()
            method()
        return mcwrap

def x():
    print 'Ala',

def y():
    print 'ma',

@depends(x,y)
def z():
    print 'kota!'
    
def a():
    print 'A',

@depends_callonce(a)
def b():
    print 'B',

@depends_callonce(b)
def c1():
    print 'C1',

@depends_callonce(b)
def c2():
    print 'C2',

def c3():
    print 'C3',
    
@depends_callonce(a)
def c4():
    print 'C4',

@depends_callonce(c1, c2, c3, c4)
def d():
    print 'D'
    
if __name__ == '__main__':
    # X ---> Y ---> Z
    z()
    #         /---> C1 ---\ 
    # A ---> B ---> C2 ---->
    # \             C3 ---/
    #  \----------> C4 --/
    d()
    