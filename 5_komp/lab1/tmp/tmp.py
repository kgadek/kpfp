>>> a(5)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'a' is not defined
>>> test-a(5)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'a' is not defined
>>> test.a(5)
25
>>> from test import *
>>> a(5)
25
>>> pattern = "a?";
>>> str = "b";
>>> r = re.compile(pattern);
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 're' is not defined
>>> from re import *;
>>> r = compile(pattern);
>>> r
<_sre.SRE_Pattern object at 0xb7129d90>
>>> m = r.search(str);
>>> m
<_sre.SRE_Match object at 0xb7127f38>
>>> for i in m: println i
  File "<stdin>", line 1
    for i in m: println i
                        ^
SyntaxError: invalid syntax
>>> for i in m: print(i);
... 
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: '_sre.SRE_Match' object is not iterable
>>> print("ala")
ala
>>> s = '100 NORTH MAIN ROAD'
>>> s.replace('ROAD','RD.')
'100 NORTH MAIN RD.'
>>> s
'100 NORTH MAIN ROAD'
>>> s[:-4]
'100 NORTH MAIN '
>>> s[:-1]
'100 NORTH MAIN ROA'
>>> s[:-0]
''
>>> s[:]
'100 NORTH MAIN ROAD'
>>> s[1:]
'00 NORTH MAIN ROAD'
>>> s[4:]
'NORTH MAIN ROAD'
>>> s[4:-4]
'NORTH MAIN '
>>> s[1:-1]
'00 NORTH MAIN ROA'
>>> re.sub('ROAD$', 'RD.', s)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 're' is not defined
>>> sub('ROAD$', 'RD.', s)
'100 NORTH MAIN RD.'
>>> 
>>> s
'100 NORTH MAIN ROAD'
>>> pat = "a{1}"
>>> str = "aa a a a";
>>> r = compile(pat)
>>> m = r.match(str)
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> dir(m)
['__class__', '__copy__', '__deepcopy__', '__delattr__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', 'end', 'endpos', 'expand', 'group', 'groupdict', 'groups', 'lastgroup', 'lastindex', 'pos', 're', 'regs', 'span', 'start', 'string']
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> m.string
'aa a a'
>>> m.start
<built-in method start of _sre.SRE_Match object at 0xb712cbf0>
>>> dir(r.match)
['__call__', '__class__', '__delattr__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__module__', '__name__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__self__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__']
>>> r.match.__doc__
'match(string[, pos[, endpos]]) --> match object or None.\n    Matches zero or more characters at the beginning of the string'
>>> ,
  File "<stdin>", line 1
    ,
    ^
SyntaxError: invalid syntax
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> print m
  File "<stdin>", line 1
    print m
          ^
SyntaxError: invalid syntax
>>> print(m)
<_sre.SRE_Match object at 0xb712cbf0>
>>> dir(m)
['__class__', '__copy__', '__deepcopy__', '__delattr__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', 'end', 'endpos', 'expand', 'group', 'groupdict', 'groups', 'lastgroup', 'lastindex', 'pos', 're', 'regs', 'span', 'start', 'string']
>>> m.expand
<built-in method expand of _sre.SRE_Match object at 0xb712cbf0>
>>> m.expand()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: expand() takes exactly one argument (0 given)
>>> m.expand.__doc__
>>> dir(m.expand)
['__call__', '__class__', '__delattr__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__module__', '__name__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__self__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__']
>>> m.groups()
()
>>> m.groups()
()
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> str
'aa a a a'
>>> pat
'a{1}'
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> m
<_sre.SRE_Match object at 0xb712cbf0>
>>> dir(m)
['__class__', '__copy__', '__deepcopy__', '__delattr__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', 'end', 'endpos', 'expand', 'group', 'groupdict', 'groups', 'lastgroup', 'lastindex', 'pos', 're', 'regs', 'span', 'start', 'string']
>>> m.span
<built-in method span of _sre.SRE_Match object at 0xb712cbf0>
>>> m.span()
(0, 1)
>>> ls
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'ls' is not defined
>>> str = "Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki."
>>> str
'Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki.'
>>>str
'Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki.'
>>> str = "Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki. Konrad nie jest bogatszy od Jurkowskiego."
>>> compile(r'(?<=Konrad )Gądek')
<_sre.SRE_Pattern object at 0xb713c190>
>>> compile(r'(?<=Konrad )Gądek').match(str)
>>> compile(r'(?<=Konrad )Gądek').match(str).groups
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'NoneType' object has no attribute 'groups'
>>> compile(r'(?<=Konrad )Gądek').match(str)
>>> compile(r'Gądek').match(str)
>>> compile(r'Gądek').search(str)
<_sre.SRE_Match object at 0xb7127f38>
>>> compile(r'Gądek').search(str).groups
<built-in method groups of _sre.SRE_Match object at 0xb712cbb8>
>>> compile(r'Gądek').search(str).groups()
()
>>> compile(r'(Gądek)').search(str).groups()
('Gądek',)
>>> compile(r'(Gądek)').search(str).groups()
('Gądek',)
>>> compile(r'Konrad?').search(str).groups()
()
>>> compile(r'(Konrad?)').search(str).groups()
('Konrad',)
>>> compile(r'(Konrad?)').search(str).groups()
('Konrad',)
>>> compile(r'(?:Konrad?)').search(str).groups()
()
>>> compile(r'(Konrad?)').search(str).groups()
('Konrad',)
>>> compile(r'(?:Konrad)( )').search(str).groups()
(' ',)
>>> compile(r'(Konrad)( )').search(str).groups()
('Konrad', ' ')
>>> str
'Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki. Konrad nie jest bogatszy od Jurkowskiego.'
>>> compile(r'Konrad|Igor').search(str).groups()
()
>>> compile(r'(Konrad)|(Igor)').search(str).groups()
('Konrad', None)
>>> compile(r'(Konrad)|(Igor)').search(str).groups()
('Konrad', None)
>>> compile(r'(Konrad)|(Igor)').search(str).group(0)
'Konrad'
>>> compile(r'(Konrad)|(Igor)').search(str).group(1)
'Konrad'
>>> compile(r'(Konrad)|(Igor)').search(str).group(2)
>>> compile(r'(Konrad)|(Igor)').search(str).groups()
('Konrad', None)
>>> str2 = 'docs.python.org/library/re.html#search-vs-match
  File "<stdin>", line 1
    str2 = 'docs.python.org/library/re.html#search-vs-match
                                                          ^
SyntaxError: EOL while scanning string literal
>>> str2 = 'docs.python.org/library/re.html#search-vs-match'
>>> str
'Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki. Konrad nie jest bogatszy od Jurkowskiego.'
>>> str2
'docs.python.org/library/re.html#search-vs-match'
>>> compile(r'').match(str2).groups()
()
>>> compile(r'(Igor|Konrad)').match(str).groups()
('Konrad',)
>>> compile(r'(Igor|Konrad)').match(str).group(0)
'Konrad'
>>> compile(r'(Igor|Konrad)').match(str).group(1)
'Konrad'
>>> compile(r'(Igor|Konrad)').match(str).group(2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: no such group
>>> str
'Konrad Gądek ma 123,56zł natomiast Igor Jurkowski ma 1123,58zł na drobne wydatki. Konrad nie jest bogatszy od Jurkowskiego.'
>>> compile('(Igor|Konrad)').match(str).group(2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: no such group
>>> compile('(Igor|Konrad)').match(str).groups()
('Konrad',)
>>> compile('(Igor|Konrad)').search(str).groups()
('Konrad',)
>>> compile(r'(Igor|Konrad)').search(str).groups()
('Konrad',)
>>> compile(r'(Igor|Konrad)').search(str).group(0)
'Konrad'
>>> compile(r'(Igor|Konrad)').search(str).group(1)
'Konrad'
>>> compile(r'(Igor|Konrad)').search(str).group(2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: no such group
>>> compile(r'(Igor|Konrad)').search(str).groups()
('Konrad',)
>>> compile(r'(Igor|Konrad)').search(str).groups()
('Konrad',)
>>> search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
  File "<stdin>", line 1
    search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
                                                                 ^
SyntaxError: invalid syntax
>>> re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
  File "<stdin>", line 1
    re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
                                                                    ^
SyntaxError: invalid syntax
>>> import re
>>> re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
  File "<stdin>", line 1
    re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$'), 'MCMXL')
                                                                    ^
SyntaxError: invalid syntax
>>> re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$', 'MCMXL')
<_sre.SRE_Match object at 0xb713a968>
>>> re.search('^M?M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$', 'MCMXL').groups()
('CM', 'XL')
>>> re.search('^M{,4}(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)$', 'MCMXL').groups()
('CM', 'XL')
>>> re.search('^M{,4}(CM|CD|D?C{,3})(XC|XL|L?X{,3})$', 'MCMXL').groups()
('CM', 'XL')
>>> re.search('^M{,4}(CM|CD|D?C{,3})(XC|XL|L?X{,3})$', 'MDCCXL').groups()
('DCC', 'XL')
>>> re.search('^M{,4}(CM|CD|D?C{,3})(XC|XL|L?X{,3})$', 'MDXL').groups()
('D', 'XL')
>>> re.sub('d','b','ddog')
'bbog'
>>> re.sub('d','b','ddog',1)
'bdog'
>>> 2
2
>>> 2+2
4
>>> 