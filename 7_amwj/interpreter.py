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

class Type(object):
	NULL=0
	VART=1
	VARS=2
	INT=3
	STR=4
	REF=5

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
	t.value = (Type.NULL, 0)
	return t


########################################################################################################################
########################################################################################################################
########################################################################################################################


heaphead  = 1
heapsize  = 81
heap      = [0 for i in range(heapsize)]
heaphalf  = heapsize / 2
heapend   = heaphalf + 1

variables = {}

class NotEnoughMemoryException(Exception): pass
class NullPointerException(Exception): pass

def allocate(asize):
	global heaphead
	if heaphead + asize > heapend:
		gc()
		if heaphead + asize > heapend:
			raise NotEnoughMemoryException()
	ret = heaphead
	heaphead = heaphead + asize
	return ret

def gc():
	pass

class SPrintLiteral(object):
	def __str__(self): return "(Print '%s')" % (self.literal,)
	def __init__(self, literal):
		self.literal = literal
	def run(self):     print self.literal

class SPrintVar(object):
	def __str__(self): return "(Print %s)" % (self.varname,)
	def __init__(self, varname):
		self.varname = varname
	def run(self):
		ptr = variables[self.varname]
		if ptr == 0: print "NULL"
		elif heap[ptr] == Type.VART: print "T{f1={0}, f2={1}, data={2}}".format(heap[ptr+1], heap[ptr+2], heap[ptr+3])
		elif heap[ptr] == Type.VARS: print "S:'{0}'".format(''.join(heap[ptr+2:ptr+2+heap[ptr+1]]))

class SVarTDecl(object):
	def __str__(self): return "(VarDeclT %s)" % (self.varname,)
	def __init__(self, varname):
		self.varname = varname
	def run(self):
		ptr = allocate(4)
		heap[ptr:ptr+4] = [Type.VART,0,0,0]
		variables[self.varname] = ptr

class SVarSDecl(object):
	def __str__(self): return "(VarDeclS %s = %s)" % (self.varname,self.value)
	def __init__(self, varname, value):
		self.varname = varname
		self.value = value
	def run(self):
		if self.value == 0:
			variables[self.varname] = 0
		else:
			ptr = allocate(2+len(self.value))
			heap[ptr] = Type.VARS
			heap[ptr+1] = len(self.value)
			heap[ptr+2:ptr+2+len(self.value)] = self.value
			variables[self.varname] = ptr

def getRef(ref):
	if ref[0] == Type.INT:
		return ref[1]
	elif ref[0] == Type.STR:
		return ref[0]
	else:
		ptr = variables[ref[1]]
		ptrv = ptr
		for x in ref[2]:
			assert ptr != 0;
			assert heap[ptrv] == Type.VART
			if   x == 'f1':
				ptr = ptrv+1
				ptrv = heap[ptr]
			elif x == 'f2':
				ptr = ptrv+2
				ptrv = heap[ptr]
			elif x == 'data':
				ptr = ptrv+3
				if ptr < len(heap): ptrv = heap[ptr]
				else:               ptrv = 0
		print "GetRef {0} --> ptr={1}".format(ref,ptr)
		return ptr

class SAssignment(object):
	def __str__(self): return "(Assignment %s = %s)" % (self.varname,self.value)
	def __init__(self, varname, value):
		self.varname = varname
		self.value = value
	def run(self):
		print "Assignment {0}::{1}   =   {2}::{3}".format(self.varname, type(self.varname), self.value, type(self.value))
		lval = getRef(self.varname)
		rval = getRef(self.value)
		if type(rval) == str:
			nptr = allocate(2+len(rval))
			heap[nptr] = Type.VARS
			heap[nptr+1] = len(rval)
			heap[nptr+2:nptr+2+len(rval)] = rval
			variables[lval] = nptr
		else:
			assert lval != 0
			if type(rval) == int:
				heap[lval] = rval
			else:
				heap[lval] = heap[rval]

class SHeapAnalyse(object):
	def __str__(self): return "(HeapAnalyse)"
	def run(self):
		print "Heap={0} # {1}\nHeapHead={2}\nVariables={3}".format(
			zip(range(heaphead),heap[:heaphead]),
			zip(range(heaphead,100000), heap[heaphead:]),
			heaphead,
			variables)

class SCollect(object):
	def __str__(self): return "(Collect)"
	def run(self):
		print "Here run GC"


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
	if len(p) == 4:
		p[0] = (Type.STR, p[2])
	elif len(p) == 2:
		if type(p[1]) == tuple:
			p[0] = p[1]
		elif type(p[1]) == int:
			p[0] = (Type.INT, p[1])

def p_deref(p):
	"""deref : STRINGCONST
					 | deref '.' STRINGCONST"""
	if   len(p) == 2:
		p[0] = (Type.REF, p[1], [])
	elif len(p) == 4:
		p[0] = (Type.REF, p[1][1], p[1][2]+[p[3]])

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
  	for i in result:
  		print "\n\n>>> {0}".format(i)
  		print "<<< {0}".format(i.run())