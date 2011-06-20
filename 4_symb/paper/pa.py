#!/usr/bin/env python
# -*- coding: utf-8

import math

conjugation = {} # słownik odmian
artnums = {} # numery artykułów
wordstats = {} # statystyki słów, np 'cow' --> {idArt1:count1, idArt4:count4, idArt15:count15}

def loadConj(filename = 'odm.txt'):
    """ Ładuje słownik odmian językowych z pliku do globalnego
    słownika conjugation. """
    try:
        file = open(filename, 'r')
        for line in file:
            parsed = line.strip().lower().split(', ')
            if len(parsed) > 1:
                for odmiana in parsed[1:]:
                    conjugation[odmiana] = parsed[0]
    finally:
        file.close()


def unpunctuate(word):
    """ Usuwa wszelkie znaki interpunkcyjne ze słowa. """
    endings = []
    changed = True
    while changed and word:
        changed = False
        for i in ['.', ',', ':', ';', '!', '?', '-']:
            if word.endswith(i):
                changed = True
                endings.append(i)
                word = word[:-len(i)]
    return word

def unconjugate(word):
    """ Zwraca bezokolicznik słowa (o ile istnieje w bazie). """
    if conjugation.has_key(word):
        word = conjugation[word]
    return word

def unconjugate_withpunct(word):
    """ Zwraca bezokolicznik słowa (o ile istnieje w bazie)
    zachowując znaki interpunkcyjne. """
    endings = []
    changed = True
    while changed and word:
        changed = False
        for i in ['.', ',', ':', ';', '!', '?', '-']:
            if word.endswith(i):
                changed = True
                endings.append(i)
                word = word[:-len(i)]
    endings.reverse()
    return unconjugate(word) + ''.join(endings)

def genWordStats(filename = 'papS.txt'):
    """ Generuje statystyki słów z artykułów. """
    try:
        file = open(filename, 'r')
        i = 'none'
        artnums[i] = 0
        for line in file:
            line = line.strip()
            if line.startswith('#'):
                print '[%s]' % (line, ),
                i = line
                artnums[i] = 0
            else:
                for word in line.lower().split(' '):
                    artnums[i] = artnums[i] + 1
                    origin = unconjugate(unpunctuate(word))
                    if not wordstats.has_key(origin):
                        wordstats[origin] = {}
                    if not wordstats[origin].has_key(i):
                        wordstats[origin][i] = 0
                    wordstats[origin][i] = wordstats[origin][i] + 1
    finally:
        file.close()

def tfidf(word,artno):
    """ Zwraca wartość wagi tf-idf danego słowa. """
    if wordstats.has_key(word) and wordstats[word].has_key(artno):
        #print 'tfidf word=%s artno=%s' % (word,artno)
        tf = 1.0 * wordstats[word][artno] / artnums[artno]
        idf = math.log(1.0 * len(artnums) / (1+len(wordstats[word])))
        return 1.0*tf*idf
    else:
        return 0.0
    
def findDescWord(filename = 'papS.txt', nbest = 5):
    """ Znajduje N słów najlepiej opisujących dany artykuł. """
    try:
        file = open(filename, 'r')
        i = 'none'
        words = {}
        for line in file:
            line = line.strip()
            if line.startswith('#'):
                print '========================= %s =========================' % i
                for i in sorted(words, key=lambda x: words[x], reverse=True)[:nbest]:
                    print '%s%8.5f' % (i.ljust(40), words[i])
                i = line
                words = {}
            else:
                for word in line.lower().split(' '):
                    origin = unconjugate(unpunctuate(word))
                    if not words.has_key(origin):
                        words[origin] = tfidf(origin, i)
    finally:
        file.close()

def main():
    print 'Conjugations...'
    loadConj('odm.txt')
    print 'Generating stats...'
    genWordStats('pap.txt')
    print '\nGenerating best words...'
    findDescWord('pap.txt')
    #try:
    #    papfp = open('pap.txt', 'r')
    #    for line in papfp:
    #        if line.startswith("#"):
    #            print line
    #        else:
    #            lineprint=[]
    #            for word in line.strip().split(' '):
    #                lineprint.append(translateword(word))
    #            print ' '.join(lineprint)
    #finally:
    #    papfp.close()

if __name__ == '__main__':
    main()