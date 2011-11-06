def Add(p,q): return ['+', p, q]
def Sub(p,q): return ['-', p, q]
def Mult(p,q): return ['*', p, q]
def Div(p,q): return ['/', p, q]
def Minus(p): return ['_', p, 0]

def eval(ex):
    if type(ex) != list: return ex
    return {
        '+' : lambda x,y: x+y,
        '-' : lambda x,y: x-y,
        '*' : lambda x,y: x*y,
        '/' : lambda x,y: x/y,
        '_' : lambda x,y: -x
        }[ex[0]](eval(ex[1]), eval(ex[2]))

def simplify_expr(ex):
    if type(ex) != list: return ex
    ex[1] = simplify_expr(ex[1])
    ex[2] = simplify_expr(ex[2])
    if ex[0] == '-' and (ex[1] == 0 or ex[2] == 0): return ex[1+int(ex[2]==0)]
    if ex[0] == '*' and (ex[1] == 1 or ex[2] == 1): return ex[1+int(ex[2]==1)]
    return ex
