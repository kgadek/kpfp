import ply.lex as lex
import ply.yacc as yacc

def p_root(p):
    """root : publications"""
    p[0] = p[1]
    ids = {}
    for i in p[0]:
        print("%s" % i)
        if i.id in ids:
            print("ERR: id %s sie powtorzyl (po raz pierwszy w %s, teraz w %s)" % (i.id, ids[i.id], i.type))
        ids[i.id] = i.type

def p_publications(p):
    """publications : publication
                    | publications publication"""
    if len(p) == 2:
        p[0] = [p[1]]
    else:
        p[0] = p[1]
        p[0].append(p[2])

class publication:
    def __init__(self, type, id, fields):
        self.type, self.id, self.fields = type, id, fields
    def __str__(self):
        res = "%s{'%s': {\n" % (self.type, self.id)
        res2 = []
        for i in self.fields:
            res2.append("\t%s : '%s'" % (("'"+i.name+"'").ljust(12), i.val))
        for i in res2[:-1] :
            res += i
            res += ",\n"
        res += res2[-1]
        res += "\n\t}\n}\n"
        return res
        
def p_publication(p):
    """publication : AT TYPE LCBRACKET ID CMMA fields RCBRACKET"""
    flds = {}
    for i in p[6]:
        if i.name in flds:
            print("ERR: w %s o id %s zadeklarowano %s ponownie\nERR:   stara wartosc: %s\nERR:   nowa wartosc: %s"
                  % (p[2], p[4], i.name, flds[i.name], i.val))
        flds[i.name] = i.val
    p[0] = publication(p[2], p[4], p[6])

def p_fields(p):
    """fields : field
              | fields CMMA field"""
    if len(p) == 2:
        p[0] = [p[1]]
    else:
        p[0] = p[1]
        p[0].append(p[3])

class field:
    def __init__(self, name, val):
        self.name, self.val = name, val
        
def p_field(p):
    """field : name EQ QUOTE VAL QUOTE
             | name EQ NUM"""
    if len(p)==6:
        p[0] = field(p[1].lower(), p[4])
    else:
        p[0] = field(p[1].lower(), p[3])

def p_name(p):
    """name : NAME_AUTHOR
            | NAME_TITLE
            | NAME_PUBLISHER
            | NAME_JOURNAL
            | NAME_BOOKTITLE
            | NAME_YEAR"""
    p[0] = p[1]

def p_error(p):
    print("ERR YACC: w linii %d fragment: '%s'" % (p.lineno, p.value))
    print("ERR YACC: internals: %s" % p)
    #while 1:
    #    tok = yacc.token()
    #    print("ERR     : odrzucam %s" % tok)
    #    if not tok or tok.type == 'RBRACE': break
    #yacc.restart()
    yacc.errok()
        
states = (
    ('book', 'exclusive'),
    ('article', 'exclusive'),
    ('inproceedings', 'exclusive'),
    ('prevalue', 'exclusive'),
    ('cvalue', 'exclusive')
)

tokens = ( 'AT', 'TYPE', 'LCBRACKET', 'RCBRACKET',
           'CMMA', 'ID', 'VAL', 'QUOTE', 'EQ', 'NUM',
           'NAME_AUTHOR', 'NAME_TITLE', 'NAME_PUBLISHER',
           'NAME_JOURNAL', 'NAME_BOOKTITLE', 'NAME_YEAR')
t_ANY_ignore = ' \t\n'

t_AT = r'@'
def t_TYPE(t):
    r'Book|Article|Inproceedings'
    t.lexer.begin(t.value.lower())
    return t

t_book_article_inproceedings_LCBRACKET = r'\{'

def t_book_article_inproceedings_RCBRACKET(t):
    r'\}'
    t.lexer.begin('INITIAL')
    return t

t_prevalue_EQ = r'='
t_book_article_inproceedings_CMMA = r','
t_book_article_inproceedings_ID = r'[a-z]+'

def t_book_article_inproceedings_NAME_AUTHOR(t):
    r'[aA][uU][tT][hH][oO][rR]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_book_article_inproceedings_NAME_TITLE(t):
    r'[tT][iI][tT][lL][eE]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_book_NAME_PUBLISHER(t):
    r'[pP][uU][bB][lL][iI][sS][hH][eE][rR]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_article_NAME_JOURNAL(t):
    r'[jJ][oO][uU][rR][nN][aA][lL]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_inproceedings_NAME_BOOKTITLE(t):
    r'[bB][oO][oO][kK][tT][iI][tT][lL][eE]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_book_article_inproceedings_NAME_YEAR(t):
    r'[yY][eE][aA][rR]'
    t.lexer.push_state('cvalue')
    t.lexer.push_state('prevalue')
    return t

def t_cvalue_prevalue_QUOTE(t):
    r'"'
    t.lexer.pop_state()
    return t

def t_prevalue_NUM(t):
    r'[0-9a-zA-Z]+'
    t.lexer.pop_state()
    t.lexer.pop_state()
    return t

t_cvalue_VAL = r'[a-zA-Z,. {}()+-_:;]+'

def t_book_error(t):
    print("## blad parsowania typu Book:\t%s\n##    :\t%s" % (t,t.value))
    t.lexer.skip(1)

def t_article_error(t):
    print("## blad parsowania typu Article:\t%s\n##    :\t%s" % (t,t.value))
    t.lexer.skip(1)

def t_inproceedings_error(t):
    print("## blad parsowania typu Inproceedings:\t%s\n##    :\t%s" % (t,t.value))
    t.lexer.skip(1)

def t_INITIAL_prevalue_cvalue_error(t):
    print("## blad parsowania typu Book:\n\t%s\n##    :\t%s" % (t,t.value))
    t.lexer.skip(1)



# MAIN ==========================================
def main():
    lexer = lex.lex()
    parser = yacc.yacc()
    fh = None
    try:
        fh = open("in.in", "r")
        parser.parse(fh.read(), lexer=lexer)
    except Exception as e:
        print("err: %s" % (e,))
    finally:
        fh.close()
