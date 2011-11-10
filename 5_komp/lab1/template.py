import os
import sys
import re

def processFile(filepath):
    fp = open(filepath, 'rU')
    content = fp.read()
    
    meta = re.findall('<META NAME="(.*?)" CONTENT="(.*?)">',content)
    metadict = {}
    junk = map(lambda (k,v): metadict.update({k: v}), meta)

    zdania = re.findall('',content)
    
    fp.close()
    print("nazwa pliku:", filepath)
    print("autor:", metadict['AUTOR'])
    print("dzial:", metadict['DZIAL'])
    print("slowa kluczowe:", metadict['KLUCZOWE_1'] + ", " + metadict['KLUCZOWE_2'] + ", " + metadict['KLUCZOWE_3'])
    print("liczba zdan:")
    print("liczba skrotow:")
    print("liczba liczb calkowitych z zakresu int:")
    print("liczba liczb zmiennoprzecinkowych:")
    print("liczba dat:")
    print("liczba adresow email:")
    print("\n")

processFile("1993-01/199301030001.html")


def bla(dirname):
    print("JO")
    try:
        path = dirname #sys.argv[1]
    except Exception:
        print("Brak podanej nazwy katalogu")
        sys.exit(0)


        tree = os.walk(path)
        
        for root, dirs, files in tree:
            for f in files:
                if f.endswith(".html"):
                    filepath = os.path.join(root, f)
                    processFile(filepath)

