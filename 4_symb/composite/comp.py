#!/usr/bin/env python
# -*- coding: utf-8

class Kompozyt:
    def __init__(self): pass
    def draw(self): pass
    def add(self, graph): pass
    def remove(self, graph): pass
    def getChild(self, id): pass

class NawiasZwykly(Kompozyt):
    def __init__(self):
        self.children = []
    def draw(self):
        return "(%s)" % ', '.join([x.draw() for x in self.children if dir(x).count('draw')>0])
    def add(self, graph):
        self.children.append(graph)
        return self
    def remove(self, graph):
        self.children.remove(graph)
        return self

class NawiasKwadratowy(Kompozyt):
    def __init__(self):
        self.children = []
    def draw(self):
        return "[%s]" % ', '.join([x.draw() for x in self.children if dir(x).count('draw')>0])
    def add(self, graph):
        self.children.append(graph)
        return self
    def remove(self, graph):
        self.children.remove(graph)
        return self

class Tekst(Kompozyt):
    def __init__(self, txt):
        self.txt = txt
    def draw(self):
        return "%s" % self.txt

if __name__ == '__main__':
    nb = NawiasKwadratowy().add(NawiasZwykly().add(Tekst('ala')).add(Tekst('ma')).add(Tekst('kota')))
    nb.add(Tekst('lol'))
    print NawiasZwykly().add(nb).add(Tekst('rotfl')).draw()