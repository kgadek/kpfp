#-*- coding: utf-8 -*-
from random import randint
from sys import argv

class Obj: pass
params = Obj()

### parametry linii poleceń
params.maxXY = 100
paramsNames = ['nazwaPliku', 'iloscWierzcholkow', 'minCzasTolerowany', 'maxCzasTolerowany', 'minKosztZatrudnienia', 'maxKosztZatrudnienia', 'minZamowieniaPracownik', 'maxZamowieniaPracownik', 'minZysk', 'maxZysk', 'minLiczbaKrawedzi', 'maxLiczbaKrawedzi','minCzasPrzejazdu', 'maxCzasPrzejazdu', 'minIloscZamowien', 'maxIloscZamowien', 'minKosztBudki', 'maxKosztBudki'];

### wczytaj parametry z linii poleceń
#print "check:\tlen(argv) = %d   ==   len(paramsNames)+1 = %d" % (len(argv), len(paramsNames)+1)
assert(len(argv) == len(paramsNames) + 1)
params.nazwaPliku = argv[1] # to jako jedyne wymaga specjalnej troski (bo string)
for (i,k) in enumerate(paramsNames[1:]):
    setattr(params, k, int(argv[i+2]))

### sprawdź poprawność parametrów
assert(1 <= params.minLiczbaKrawedzi)
assert(1 <= params.iloscWierzcholkow)
for (a,b) in zip(paramsNames[2::2], paramsNames[3::2]):
    #print "check:\t0 <= %s <= %s" % (a,b,)
    assert(0 <= getattr(params, a) <= getattr(params, b))

###########################################################################
###########################################################################

def phase1a(params,X,Y,K,Z,deg,E):
    """
    Przygotuj wstępnie graf do wygenerowania połączeń:
    A) sprawdź, czy wierzchołki się nie nakładają
    B) wygeneruj losowe drzewo rozpinające
    """
    return (params,X,Y,K,Z,deg,E)

def phase2a(params,X,Y,K,Z,deg,E):
    """ Wygeneruj połączenia w grafie """
    return (params,X,Y,K,Z,deg,E)

def getStrGraphViz(params,X,Y,K,Z,deg,E):
    """ Generuje wyjście dla programu graphviz """
    return ""

def getStrShyshaCons(params,X,Y,K,Z,deg,E):
    """ Generuje wyjście do pliku *.shyshacons """
    return "%d %d %d %d %d %d %d %d %d\n" % (params.iloscWierzcholkow,
                                             params.minCzasTolerowany, params.maxCzasTolerowany,
                                             params.minKosztZatrudnienia, params.maxKosztZatrudnienia,
                                             params.minZamowieniaPracownik, params.maxZamowieniaPracownik,
                                             params.minZysk, params.maxZysk)

def getStrShyshaGraph(params,X,Y,K,Z,deg,E):
    """ Generuje wyjście do pliku *.shyshagraph """
    return ""

###########################################################################
###########################################################################

result = phase2a(*phase1a(params,
                          [randint(0, params.maxXY)] * params.iloscWierzcholkow, # X
                          [randint(0, params.maxXY)] * params.iloscWierzcholkow, # Y
                          [randint(params.minKosztBudki, params.maxKosztBudki)] * params.iloscWierzcholkow, # K
                          [randint(params.minIloscZamowien, params.maxIloscZamowien)] * params.iloscWierzcholkow, # Z
                          [0] * params.iloscWierzcholkow, # deg
                          [[0] * params.iloscWierzcholkow] * params.iloscWierzcholkow)) # E

with open(params.nazwaPliku + '.shyshacons', 'w') as f:
    f.write(getStrShyshaCons(*result))
with open(params.nazwaPliku + '.shyshagraph', 'w') as f:
    f.write(getStrShyshaGraph(*result))
