#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tester.h"

#if SYS==1
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#define DEBUG 0
#if DEBUG==1
#define DBG(x) {(x);}
#else
#define DBG(x) {}
#endif


int main(int argc, char **argv) {
	FileHandler fh;
	uint size, items, i, j;
	int tmp;

#if SYS==1 && DEBUG==1
	printf("wersja sys\n");
#elif DEBUG==1
	printf("wersja glibc\n");
#endif

	if(argc == 5 && !strcmp(argv[1],"generuj")) {
		/* ================================================================== */
		DBG(printf("generuj w pliku %s %d-struktur o rozmiarze %d\n",\
				argv[4],atoi(argv[3]),atoi(argv[2])));
		myRandInit();

		fh = myOpenFile(argv[4], MYFF_WB);	/*Korzystam z własnego API*/
		if(!myOpenedFile(fh)) {
			printf("Błąd! Nie można otworzyć pliku do zapisu\n");
			return 1;
		}

		size = (uint)atoi(argv[2]);
		items = (uint)atoi(argv[3]);
		myWriteFile(fh, &items, sizeof(uint), 1);
		myWriteFile(fh, &size, sizeof(uint), 1);

		for(i=0;i<items;++i) {
			tmp = rand();
			myWriteFile(fh, &tmp, sizeof(int), 1);
			for(j=0;j<size;++j) {
				tmp = rand();
				myWriteFile(fh, &tmp, sizeof(char), 1);
			}
		}

		myCloseFile(&fh);


	} else if (argc == 3 && !strcmp(argv[1],"sortuj")) {
		/* ================================================================== */
		DBG(printf("sortuj: %s\n", "ala"));
		fh = myOpenFile(argv[2], MYFF_RWB);
		if(!myOpenedFile(fh)) {
			printf("Błąd! Nie można otworzyć pliku do odczytu/zapisu\n");
			return 1;
		}

		myGetFileParams(fh, &items, &size); /*Wczytuję parametry z pliku*/
		DBG(printf("items=%d size=%d\n", items, size));

		do {
			tmp = 0;
			for(i=1; i<items; ++i) {
				if(myCmpKlucze(fh, size, i-1, i) > 0) {
					tmp = 1;
					mySwapWiersze(fh, i-1, i, size);
				}
			}
		} while(tmp);

		myCloseFile(&fh);


	} else {
		/* ================================================================== */
		fprintf(stderr, "Błędne wywołanie programu\n"\
				"Należy wywołać program w jeden z poniższych sposobów:\n"\
					"\t%s generuj rozmiar liczba plik\n"\
					"\t%s sortuj plik\n",\
				argv[0], argv[0]);
		return 1;
	}
	return 0;
}


/* ########################################################################## */


void myRandInit() {
	srand((unsigned int)time(0));
}

int myRandUint(int a, int b) {
	return (a>b) ? a : rand()%((int)(b-a+1)) + a;
}

FileHandler myOpenFile(const char *fn, myFileFlags fl) {
	FileHandler fh;
	switch(fl) {
#if SYS==1
		case MYFF_RB: fh = open(fn, O_RDONLY);
					  break;
		case MYFF_WB: fh = open(fn, O_WRONLY | O_CREAT, S_IRWXU );
					  break;
		case MYFF_RWB: fh = open(fn, O_RDWR | O_CREAT);
					   break;
		default: fh = -1;
#else
		case MYFF_RB: fh = fopen(fn, "rb");
				   break;
		case MYFF_WB: fh = fopen(fn, "wb");
				   	break;
		case MYFF_RWB: fh = fopen(fn, "rb+");
					   break;
		default: fh = 0;
#endif
	}
	return fh;
}

int myOpenedFile(FileHandler fh) {
#if SYS==1
	return fh != -1;
#else
	return fh != 0;
#endif
}

void myCloseFile(FileHandler *fh) {
#if SYS==1
	close(*fh);
	*fh = -1;
#else
	fclose(*fh);
	*fh = 0;
#endif
}

uint myReadFile(FileHandler fh, void *buf, uint size, uint items) {
#if SYS==1
	return (uint)read(fh, buf, size*items);
#else
	return fread(buf, size, items, fh);
#endif
}

uint myWriteFile(FileHandler fh, const void *buf, uint size, uint items) {
#if SYS==1
	return (uint)write(fh, buf, size*items);
#else
	return fwrite(buf, size, items, fh);
#endif
}

int mySeekFile(FileHandler fh, uint pos) {
#if SYS==1
	return lseek(fh, (long int)pos, SEEK_SET) != (off_t)-1;
#else
	return fseek(fh, (long int)pos, SEEK_SET) != -1;
#endif
}

long int myGetOffset(uint size, uint i) {
	return 8+(long int)(i*(sizeof(int)+size));
}

int myGetKlucz(FileHandler fh, uint size, uint i) {
	int k;
	if(mySeekFile(fh, (uint)myGetOffset(size, i)) == 0)
		return 0;
#if SYS==1
	read(fh, &k, sizeof(int));
#else
	fread(&k, sizeof(int), 1, fh);
#endif
	return k;
}

void myGetWartosc(FileHandler fh, uint i, uint size, char *buf) {
	mySeekFile(fh, (uint)myGetOffset(size,i)+(uint)sizeof(int));
	myReadFile(fh, buf, size, 1);
}

int myCmpKlucze(FileHandler fh, uint size, uint i, uint j) {
	return myGetKlucz(fh,size,i)-myGetKlucz(fh,size,j);
}

void mySwapWiersze(FileHandler fh, uint i, uint j, uint size) {
	int ik, jk;
	char *iv, *jv;
	DBG(printf("swap %d:%d <--> %d:%d\n",i,myGetKlucz(fh,size,i),j,myGetKlucz(fh,size,j)));
	if( !(iv = MEMALLOC(char,size)) || !(jv = MEMALLOC(char,size))) {
		free(iv);
		free(jv);
	}
	ik = myGetKlucz(fh, size, i); /*Ponownie czytamy wartości kluczy*/
	jk = myGetKlucz(fh, size, j);
	myGetWartosc(fh, i, size, iv);
	myGetWartosc(fh, j, size, jv);

	mySeekFile(fh, (uint)myGetOffset(size, j));
	myWriteFile(fh, &ik, sizeof(int), 1);
	myWriteFile(fh, iv, size, 1);

	mySeekFile(fh, (uint)myGetOffset(size, i));
	myWriteFile(fh, &jk, sizeof(int), 1);
	myWriteFile(fh, jv, size, 1);
	free(iv);
	free(jv);
}

void myGetFileParams(FileHandler fh, uint *n, uint *s) {
#if SYS==1
	read(fh, n, sizeof(uint));
	read(fh, s, sizeof(uint));
#else
	fread(n,sizeof(uint),1,fh);
	fread(s,sizeof(uint),1,fh);
#endif
}

