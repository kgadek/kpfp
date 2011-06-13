#!/usr/bin/env python
# -*- coding: utf-8

odmdict = {}

def translateword(word):
    """ Zamienia słowo zachowując ew. (pojedynczy) znak dodatkowy na słowo
    ze słownika.
    NOŁN BUGZ:
     - nie zachowuje wielkości liter
     - nie radzi sobie z wielokropkiem ani z kombinacjami typu '!?'
     - raczej niewydajny """
    ends = ""
    word = word.lower()
    for i in ['.', ',', '!', ':', ';']:
        if word.endswith(i):
            ends = i
            break
    if ends: word = word[:-1]
    return odmdict.has_key(word) and odmdict[word] + ends or word + ends

def main():
    try:
        odmfp = open('odm.txt', 'r')
        for line in odmfp:
            parsed = line.strip().split(', ')
            if len(parsed) > 1:
                for odmiana in parsed[1:]:
                    odmdict[odmiana] = parsed[0]
    finally:
        odmfp.close()
    try:
        papfp = open('pap.txt', 'r')
        for line in papfp:
            if line.startswith("#"):
                print line
            else:
                lineprint=[]
                for word in line.strip().split(' '):
                    lineprint.append(translateword(word))
                print ' '.join(lineprint)
    finally:
        papfp.close()

if __name__ == '__main__':
    main()