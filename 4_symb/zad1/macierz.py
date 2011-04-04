#!/usr/bin/env python
# -*- coding: utf-8

""" Operacje na macierzach.

    TODO zainstalować:
        - easy_install
        - virtual_env
"""

import types

class Callable:
    """ Pozwala na proste tworzenie metod statycznych.
    
    EXAMPLE:
    class Ala:
        @Callable
        def getAlaWrazZKotem():
            return "Ala co ma kota"
    """
    def __init__(self, any):
        self.__call__ = any

class matrix:
    """ Klasa reprezentująca pojedynczą macierz """
    
    class matrixException(Exception):
        """ Nadklasa wszystkich wyjątków """
        pass
    class matrixNotAMatrixException(matrixException):
        """ Klasa reprezentująca wyjątek, gdy dane wejściowe nie reprezentują porawnej macierzy """
        def __init__(self,value): self.value = value
        def __str__(self): return repr(self.value)
    class matrixBadDimension(matrixException):
        """ Klasa reprezentująca wyjątek, gdy wymiary macierzy nie pozwalają na wykonanie pewnej operacji """
        def __init__(self,value): self.value = value
        def __str__(self): return repr(self.value)
    
    @Callable
    def readFromFile(filename):
        """ Wczytuje macierz z pliku.
        
        IN: filename - nazwa pliku wejściowego
        OUT: obiekt klasy matrix lub None jeśli wystąpił błąd
        """
        try:
            f = open(filename)
            mat = [[float(x) for x in w.strip().split(' ')] for w in f]
            rows = mat and [len(w) for w in mat] or None
            if mat and rows.count(rows[0]) != len(rows):
                raise matrix.matrixNotAMatrixException('Dane wczytane z pliku %s nie reprezentują poprawnej macierzy.' % (filename, ))
        except IOError as (errno, strerror): print "I/O error(%s): %s" % (errno,strerror)
        except matrix.matrixException as e: print "Exception in matrixReadFromFile(%s)!\n%s" % (filename, indent(e.value))
        else: return matrix(matrix=mat, origin=filename)
        finally: f.close()
        return None
    def __init__(self, matrix=None, origin=None):
        """ Inicjalizator """
        self.matrix = matrix
        self.origin = origin
        self.rows = self.matrix and len(self.matrix)
        self.cols = self.matrix and len(self.matrix[0])
    def __repr__(self):
        """ Konwerter klasy do string """
        return self.matrix and '\n'.join(["|%s|" % s for s in [' '.join(["%-6.3f" % e for e in w]) for w in self.matrix]]) or "<pusta macierz>"
    def __getitem__(self, key):
        """ Funkcja dostępu do klasy.
        
        NOTE: Zakładamy, że macierz jest immutable """
        try:
            if key.__class__.__name__ != 'tuple' or len(key) != 2:
                raise self.matrixBadDimension("Jako parametr należy podać dwuelementową listę.\nPodano: %s" % (key,))
                return None
            elif not 0 <= key[0] < self.getRowCount or not 0 <= key[1] < self.getColCount :
                raise self.matrixBadDimension('Podano niewłaściwą współrzędną macierzy.\nPodano: %s' % (key,))
                return None
            elif not self.matrix:
                raise self.matrixBadDimension('Zażądano odwołania do współrzędnych pustej macierzy.\nPodano: %s' % (key,))
                return None
        except self.matrixException as e: print "Exception in matrix.__getitem__(%s)!\n%s" % (key, indent(e.value))
        else: return self.matrix[key[0]][key[1]]
    def __len__(self):
        return self.getRowCount()
    def getRowCount(self): return self.rows
    def getColCount(self): return self.cols
    def getDim(self):
        """ Zwraca wymiary macierzy (string) """
        return "%dx%d" % (self.rows, self.cols)
    def __cmp__(self, foreign):
        """ Sprawdza, czy dwie macierze są równe """
        return isinstance(foreign, matrix) and cmp(self.matrix, foreign.matrix)
    def getCol(self, col):
        """ Zwraca kolumnę macierzy """
        try:
            if not 0 <= col < self.getColCount:
                raise self.matrixBadDimension('Podano niewłaściwy numer kolumny macierzy.\nPodano: %s' % (col,))
                return None
        except self.matrixException as e: print "Exception in matrix.getCol(%s), self = %s!\n%s" % (col, self, indent(e.value))
        else: return [x[col] for x in self.matrix]
    def getRow(self, row):
        """ Zwraca wiersz macierzy """
        try:
            if not 0 <= row < self.getRowCount:
                raise self.matrixBadDimension('Podano niewłaściwy numer wiersza macierzy.\nPodano: %s' % (row,))
                return None
        except self.matrixException as e: print "Exception in matrix.getRow(%s), self = %s!\n%s" % (row, self, indent(e.value))
        else: return self.matrix[row]
    def mul(self, foreign):
        """ Metoda zwraca iloczyn bieżącej macierzy z inną. """
        try:
            if not isinstance(foreign, matrix):
                raise self.matrixNotAMatrixException('Argument mnożenia nie jest macierzą (obiektem typu matrix).')
            elif self.getColCount() != foreign.getRowCount():
                raise self.matrixBadDimension('Nie można pomnożyć macierzy o wymiarach %s i %s.' % (self.getDim(), foreign.getDim()))
            mat = [[reduce(lambda x,y: x+y,
                           map(lambda x,y: x*y,
                               self.getRow(i),
                               foreign.getCol(j)))
                    for j in range(foreign.getColCount())]
                for i in range(self.getRowCount())]
        except self.matrixException as e: print "Wyjątek w A.mul(B)!\nA = \n%s\nB = \n%s\n" % (indent(self), indent(foreign))
        else: return matrix(mat, "((%s) .* (%s))" % (self.origin, foreign.origin))
        return None
    def minor(self,i,j):
        """ Zwraca minor macierzy (,,skreśla'' dokładnie jeden wiersz i kolumnę). """
        return matrix([[self.getRow(a)[b] for b in range(self.getColCount()) if b != j] for a in range(self.getRowCount()) if a != i])
    def det(self, colBy = 0):
        """ Zwraca wyznacznik macierzy lub None gdy macierz nie jest kwadratowa. """
        try:
            if not 0 <= colBy < self.getColCount(): raise self.matrixBadDimension('Podano niewłaściwy numer kolumny macierzy.\nPodano: %s' % (colBy,))
            if self.getColCount() != self.getRowCount() or not self.matrix: return None
            if self.getColCount() == 1: return self[0,0]
        except self.matrixException as e:
            print "Wyjątek w A.det(colBy = %d)!\nA = \n%s\n" % (colBy, indent(self))
            return None
        else:
            return reduce(lambda x,y: x+y, [(-1)**(i+colBy) * self[i,colBy] * self.minor(i,colBy).det() for i in range(self.getColCount())])
    

def indent(inpstr, pre="\t"):
    """ Dodaje wcięcie dla każdej linii napisu wejściowego.
    
    IN: inpstr - napis
        pre - wcięcie (domyślnie: "\t")
    OUT: Napis przesunięty w prawo.
    """
    return "%s%s" % (pre, "\n\t".join(str(inpstr).split("\n")))



if __name__ == '__main__':
    a = matrix([[1,0,2],[-1,3,1]], 'A')
    b = matrix([[3,1],[2,1],[1,0]], 'B')
    c = a.mul(b)
    if c == matrix([[5,1],[4,2]]) and c != a and c != b:
        print 'OK: \n%s' % (indent(c))
    else:
        print 'ERROR'
