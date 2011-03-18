all: klucz podpis weryfikacja

klucz: klucz.c
	gcc klucz.c -o klucz -lssl

podpis: podpis.c
	gcc podpis.c -o podpis -lssl

weryfikacja: weryfikacja.c
	gcc weryfikacja.c -o weryfikacja -lssl

clean:
	rm -f klucz podpis weryfikacja