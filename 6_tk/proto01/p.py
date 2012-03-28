import ply.lex as lex
import ply.yacc as yacc

tokens = ('PLUSPLUS', 'ATOM', 'INT')
literals = '+-*/(){}[]'
t_ignore = ' \t\n'

def t_PLUSPLUS(t):
    r'\+\+'
    return t

def t_ATOM(t):
    r'[a-zA-Z]+'
    return t

def t_INT(t):
    r'[0-9]+'
    t.value = int(t.value)
    return t

def t_ANY_error(t):
    print "Illegal char"
    t.lexer.skip(1)

def t_ANY_newline(t):
    r'\n'
    t.lexer.lineno += len(t.value)


t_ANY_ignore = '\n'

def main():
    lexer = lex.lex()
    parser = yacc.yacc()
    with open("in.in") as fh:
        parser.parse(fh.read(), lexer=lexer)

def mainl():
    lexer = lex.lex()
    with open("in.in") as fh:
        lexer.input(fh.read())
        for token in lexer:
            print "line %d: %-10s --> %10s" % (token.lineno, token.type, token.value)
