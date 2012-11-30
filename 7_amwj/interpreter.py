#!/usr/bin/env python

import ply.lex as lex
import ply.yacc as yacc
import sys as sys

tokens   = ('STRINGCONST', 'INTEGERCONST', 'VARDECLT', 'VARDECLS', 'HEAPANALYSE', 'COLLECT', 'PRINT', 'NULL')
literals = ';=."'

t_ignore = ' \t'

t_VARDECLT    = r'VarDeclT'
t_VARDECLS    = r'VarDeclS'
t_HEAPANALYSE = r'HeapAnalyze' # GB > USA
t_COLLECT     = r'Collect'
t_PRINT       = r'Print'


########################################################################################################################
########################################################################################################################
########################################################################################################################


def t_error(t):
  print "Illegal character '%s'" % t.value[0]
  t.lexer.skip(1)

def t_newline(t):
  r'\n+'
  t.lexer.lineno += len(t.value)

def t_STRINGCONST(t):
  r'[a-z][a-zA-Z0-9]*'
  return t

def t_INTEGERCONST(t):
  r'\d+'
  t.value = int(t.value)
  return t

def t_NULL(t):
	r'NULL'
	t.value = 0
	return t


########################################################################################################################
########################################################################################################################
########################################################################################################################


variables = {}

class SDeref(object):
	def __init__(self, p, q):
		self.p = p
		self.q = q
	def __str__(self): return "(Deref %s.%s)" % (self.p,self.q)
	def run(self):
		print "%s -> %s" % (p, q)	

class SPrintLiteral(object):
	def __init__(self, literal):
		self.literal = literal
	def __str__(self): return "(Print '%s')" % (self.literal,)
	def run(self):
		print self.literal

class SPrintVar(object):
	def __init__(self, varname):
		self.varname = varname
	def __str__(self): return "(Print %s)" % (self.varname,)
	def run(self):
		print variables[self.varname]

class SVarTDecl(object):
	def __init__(self, varname):
		self.varname = varname
	def __str__(self): return "(VarDeclT %s)" % (self.varname,)
	def run(self):
		variables[self.varname] = self

class SVarSDecl(object):
	def __init__(self, varname, value):
		self.varname = varname
		self.value = value
	def __str__(self): return "(VarDeclS %s = %s)" % (self.varname,self.value)
	def run(self):
		variables[self.varname] = self

class SAssignment(object):
	def __init__(self, varname, value):
		self.varname = varname
		self.value = value
	def __str__(self): return "(Assignment %s = %s)" % (self.varname,self.value)
	def run(self):
		variables[self.varname] = self.value

class SHeapAnalyse(object):
	def __str__(self): return "(HeapAnalyse)"
	def run(self): pass

class SCollect(object):
	def __str__(self): return "(Collect)"
	def run(self): pass

########################################################################################################################
########################################################################################################################
########################################################################################################################


def p_program(p):
  """program : statements"""
  p[0] = p[1]

def p_statements(p):
  """statements : statement statements
                | statement
                |"""
  try:
	  p[0] = p[1:2]
	  p[0].extend(p[2])
  except Exception, e: pass

def p_statement(p): 
  """statement : variable_declaration ';'
               | assignment ';'
               | PRINT '"' STRINGCONST '"' ';'
               | PRINT STRINGCONST ';'
               | HEAPANALYSE ';'
               | COLLECT ';'"""
  if len(p)   == 3:
  	if p[1]   == "HeapAnalyze": p[0] = SHeapAnalyse()
  	elif p[1] == "Collect":     p[0] = SCollect()
  	else:                       p[0] = p[1]
  elif len(p) == 4:             p[0] = SPrintVar(p[2])
  elif len(p) == 6:             p[0] = SPrintLiteral(p[3])

def p_variable_declaration(p):
	"""variable_declaration : VARDECLT STRINGCONST
	                        | VARDECLS STRINGCONST '"' STRINGCONST '"'
	                        | VARDECLS STRINGCONST NULL
	                        """
	if   len(p) == 3: p[0] = SVarTDecl(p[2])
	elif len(p) == 4: p[0] = SVarSDecl(p[2], 0)
	elif len(p) == 6: p[0] = SVarSDecl(p[2], p[4])

def p_assignment(p):
	"""assignment : lvalue '=' rvalue"""
	p[0] = SAssignment(p[1], p[3])

def p_lvalue(p):
	"""lvalue : deref"""
	p[0] = p[1]

def p_rvalue(p):
	"""rvalue : deref
						| NULL
						| '"' STRINGCONST '"'
						| INTEGERCONST"""
	if   len(p) == 4: p[0] = p[2]
	elif len(p) == 2: p[0] = p[1]

def p_deref(p):
	"""deref : STRINGCONST
					 | deref '.' STRINGCONST"""
	if   len(p) == 2: p[0] = p[1]
	elif len(p) == 4: p[0] = SDeref(p[1], p[3])

def p_error(p):
	print "Syntax error in line %d" %  p.lineno


########################################################################################################################
########################################################################################################################
########################################################################################################################


if __name__ == '__main__':
  lexer = lex.lex()
  parser = yacc.yacc()
  with open(sys.argv[1], "r") as fh:
  	result = parser.parse(fh.read(), lexer=lexer)
  	print result
  	for i in result:
  		print i
  		i.run() 