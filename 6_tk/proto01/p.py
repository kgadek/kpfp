import ply.lex as lex
import ply.yacc as yacc



#############################################################################
# Lexer                                                                     #
#############################################################################

tokens = ('ATOM', 'INT')
literals = ['+', '-', '(', ')']
#literals = '+-*/()'
#precedence = (
#    # im niżej tym wyższy priorytet
#    ("left", '+', '-'),
#    ("left", '*', '/'),
#)
t_ignore = ' \t\n'
#states = (
#    ('lisp', 'exclusive'),
#    ('proc', 'exclusive'),
#)

def t_ANY_ATOM(t):
    r'[a-zA-Z]+'
    return t

def t_ANY_INT(t):
    r'[0-9]+'
    t.value = int(t.value)
    return t

def t_ANY_error(t):
    print "Illegal: '%s'" % t.value[0]
    t.lexer.skip(1)

def t_ANY_newline(t):
    r'\n'
    t.lexer.lineno += len(t.value)



#############################################################################
# Parser                                                                    #
#############################################################################

def p_root(p):
    """ root : sexp
             | sexp root """
    ret = [ 'progn' ]
    ret.extend(p[1:])
    p[0] = ret
    print "p_root = %s" % p[0]

def p_sexp(p):
    """ sexp : '(' parlist ')' """
    p[0] = p[2]

def p_parlist(p):
    """ parlist : expression
                | expression parlist """
    print "in p_parlist: p[1]=%s p[2:]=%s" % (p[1], p[2:])
    p[0] = [ p[1] ]
    if len(p) > 2:        
        p[0].extend(p[2])
    print "p_parlist = %s" % p[0]

def p_exp(p):
    """ expression : primitive
                   | sexp """
    p[0] = p[1]
    print "p_exp = %s" % p[0]

def p_primitive(p):
    """ primitive : ATOM
                  | INT
                  | basicfun """
    p[0] = p[1]
    print "p_primitive = %s" % p[0]

def p_basicfun(p):
    """ basicfun : '+'
                 | '-'
                 | '*'
                 | '/' """
    p[0] = p[1]
    print "p_basicfun = %s" % p[0]

def p_error(p):
    print "syntax error in line %3d: %s // %s" % (p.lineno, str(p), str(yacc))
    yacc.errok()

#############################################################################
# Program                                                                   #
#############################################################################

def ex(exp):
    if type(exp) is list:
        exp = map(ex, exp)
        return { '+' : lambda: sum(exp[1:]),
                 '-' : lambda: exp[1]-sum(exp[2:]),
                 'progn' : lambda: map(ex, exp[1:])
               }[exp[0]]()
    else:
        return exp

def mainp():
    lexer = lex.lex()
    parser = yacc.yacc()
    res = []
    with open("in.in") as fh:
        res = parser.parse(fh.read())
        print res
    return res

def mainl():
    lexer = lex.lex()
    with open("in.in") as fh:
        lexer.input(fh.read())
        for token in lexer:
            print "line %d: %-10s -->  %s" % (token.lineno, token.type, token.value)

