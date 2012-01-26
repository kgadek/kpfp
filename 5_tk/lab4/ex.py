import ply.lex as lex

states = (
    ('book', 'exclusive'),
    ('value', 'exclusive')
)

tokens = ( 'MALPA', 'TYP',
           'LNAWIAS', 'RNAWIAS',
           'ROWNOSC', 'PRZECINEK', 'CUDZYSLA', 'CUDZYSLB', 'POLE', 'CONTENT', 'LICZBA')
t_ANY_ignore = ' \t\n'

t_MALPA = r'\@'
t_TYP = r'Book|Article|Inproceedings'
t_book_ROWNOSC = r'='
t_book_PRZECINEK = r'\,'
t_book_POLE = r'[a-z]+'
t_value_CONTENT = r'[a-zA-Z \t.,]+'

def t_book_CUDZYSLA(t):
    r'\"'
    t.lexer.push_state('value')
    return t

def t_value_CUDZYSLB(t):
    r'\"'
    t.lexer.pop_state()
    return t

def t_LNAWIAS(t):
    r'\{'
    t.lexer.begin('book')
    return t

def t_book_RNAWIAS(t):
    r'\}'
    t.lexer.begin('INITIAL')
    return t

def t_book_LICZBA(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_book_newline(t):
    r'\n'
    t.lexer.lineno += len(t.value)

def t_ANY_error(t):
    print("Illegal char")
    t.lexer.skip(1)

def main():
    lexer = lex.lex()
    fh = None
    try:
        fh = open("in.in", "r")
        lexer.input(fh.read())
        for token in lexer:
            print("line %d: %s(%s)" % (token.lineno, token.type, token.value))
    except Exception as e:
        print("err: %s" % (e,))
    finally:
        fh.close()
