#!/usr/bin/env python
# -*- coding: utf-8

from weakref import WeakValueDictionary

class BigObj(object):
    inst = WeakValueDictionary()
    def __new__(cls, id):
        if not cls.inst.has_key(id):
            a = object.__new__(cls)
            cls.inst[id] = a
        return cls.inst[id]
    def __init__(self, haslo):
        self.haslo = haslo
    def getHaslo(self):
        return self.haslo
    def setHaslo(self, newHaslo):
        oldHaslo = self.haslo
        self.haslo = newHaslo
        return oldHaslo

if __name__ == '__main__':
    print "Start"
    a = BigObj(2)
    print "a=%s" % (a.getHaslo())
    b = BigObj(2)
    print "b=%s" % (b.getHaslo())
    b.setHaslo(3)
    print "a=%s" % (a.getHaslo())
    print "b=%s" % (b.getHaslo())
    