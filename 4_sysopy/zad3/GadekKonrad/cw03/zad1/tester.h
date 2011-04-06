#ifndef _TESTER_H
#define _TESTER_H

#include <stdio.h>

#if SYS == 1
typedef int FileHandler;
#else
typedef FILE* FileHandler;
#endif

#define MEMALLOC(typ,times) ((typ*)malloc(((uint)(times))*sizeof(typ)))

typedef unsigned int uint;

typedef struct {
	int k;
	void *v;
} block;

typedef enum {
	MYFF_RB,
	MYFF_WB,
	MYFF_RWB
} myFileFlags;

/**
  * Inicjuje generator liczb losowych.
  */
void myRandInit(void);

/**
  * Zwraca liczbę z przedziału [A,B] lub A gdy A>B.
  */
int myRandUint(int a, int b);

/**
  * Otwiera plik.
  */
FileHandler myOpenFile(const char *fn, myFileFlags fl);

/**
  * Stwierdza, czy powiodło się otwarcie (1) czy nie (0).
  */
int myOpenedFile(FileHandler fh);

/**
  * Zamyka plik.
  */
void myCloseFile(FileHandler *fh);

/**
  * Wczytuje do bufora *buf items bloków o rozmiarze size każdy z
  * pliku o deskryptorze fh.
  */
uint myReadFile(FileHandler fh, void *buf, uint size, uint items);

/**
  * Zapisuje do pliku o deskryptorze fh items elementów o rozmiarze
  * size każdy odczytanych z bufora *buf.
  */
uint myWriteFile(FileHandler fh, const void *buf, uint size, uint items);

/**
  * Przesuwa wskaźnik w pliku o deskryptorze fh na miejsce pos.
  * Potrzebuje informacji o rozmiarze bloku danych.
  */
int mySeekFile(FileHandler fh, uint i);

/**
  * Wyznacza offset w pliku i-tego wiersza
  */
long int myGetOffset(uint size, uint i);

/**
  * Zwraca wartość klucza z linii numer i;
  */
int myGetKlucz(FileHandler fh, uint size, uint i);

/**
  * Wczytuje wartość z linii i do bufora *buf.
  */
void myGetWartosc(FileHandler fh, uint i, uint size, char *buf);

/**
  * Stwierdza, która linia jest ,,mniejsza''.
  */
int myCmpKlucze(FileHandler fh, uint size, uint i, uint j);

/**
  * Zamienia wiersze i oraz j miejscami.
  */
void mySwapWiersze(FileHandler fh, uint i, uint j, uint size);

/**
  * Wczytuje z pliku jego parametry: ilość wierszy (*n) oraz długość wektora bajtów (*s).
  * UWAGA: zakłada, że fh jest ustawiony na początek pliku.
  */
void myGetFileParams(FileHandler fh, uint *n, uint *s);

#endif

