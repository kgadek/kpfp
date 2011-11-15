# -*- coding: utf-8
import os
import sys
import re

def processFile(filepath):
    fp = open(filepath, 'rU')
    content = fp.read()
    fp.close()
    
    meta = re.findall('<META NAME="(.*?)" CONTENT="(.*?)">',content)
    metadict = {}
    junk = map(lambda (k,v): metadict.update({k: v}), meta)

    # make it smoooooooooooth
    start = re.search('<P>',content).start()
    end = re.search('<META ',content[start:]).start()+start
    content = content[start:end]
    content = re.sub('[ 	]+',' ',content)
    content = re.sub('<.+?>','',content)
    content = re.sub('\n+','\n',content) + ' '

    #lines = filter(lambda x: x != '', content.split('\n'))
    sentences1 = re.findall('[\wżółćęąźń]{4,}[         ]*[\.\n]', content, re.UNICODE)
    sentences2 = re.findall('\s[\wżółćęąźń]{1,3}[      ]*[\.\n][        ]*[A-Z\n]', content, re.UNICODE)
    shortcs = re.findall('\s(\w{1,3}\.)\s', content, re.UNICODE)
    ints = re.findall('\s(-?\d{1,5})\s', content, re.UNICODE)
    floats = re.findall('\s(\d+\.\d*)\s', content, re.UNICODE)
    dates1 = re.findall('\d{4}[-/.]\d{2}[-/.]\d{2}',content, re.UNICODE)
    dates2 = re.findall('\d{2}[-/.]\d{2}[-/.]\d{4}',content, re.UNICODE)
    emails = re.findall('(\S+?@\S+?\.\S+)', content, re.UNICODE)

    #print content

    print("nazwa pliku:", filepath)
    print("autor:", metadict['AUTOR'])
    print("dzial:", metadict['DZIAL'])
    print("slowa kluczowe:", metadict['KLUCZOWE_1'] + ", " + metadict['KLUCZOWE_2'] + ", " + metadict['KLUCZOWE_3'])
    print("liczba zdan:", len(sentences1) + len(sentences2))
    print("liczba skrotow:", len(shortcs))
    print("liczba liczb calkowitych z zakresu int:", len(ints))
    print("liczba liczb zmiennoprzecinkowych:", len(floats))
    print("liczba dat:", len(dates1) + len(dates2))
    print("liczba adresow email:", len(emails))
    print("\n")

#processFile("1993-01/199301030001.html")


def bla():
    try:
        for (root, dirs, files) in os.walk(sys.argv[1]):
            for f in files:
                if f.endswith(".html"):
                    filepath = os.path.join(root, f)
                    processFile(filepath)

    except Exception:
        print("Brak podanej nazwy katalogu")
        sys.exit(0)

if __name__ == "__main__":
    bla()
