def Add(p,q): return ['+', p, q]
def Sub(p,q): return ['-', p, q]
def Mult(p,q): return ['*', p, q]
def Div(p,q): return ['/', p, q]
def Minus(p): return ['_', p]

def eval(ex):
    if type(ex) != list: return ex
    if ex[0] == '_': return -eval(ex[1])
    return {
        '+' : lambda x,y: x+y,
        '-' : lambda x,y: x-y,
        '*' : lambda x,y: x*y,
        '/' : lambda x,y: x/y
        }[ex[0]](eval(ex[1]), eval(ex[2]))
