#ifndef COMMON_H_
#define COMMON_H_

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define myerror(str,myerrno) {\
			fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
			exit(myerrno);\
		}

typedef unsigned int uint;

/*ilość producentów*/
#define prodCnt 5
/*ilość semaforów*/
#define semCnt 3
/*rozmiar macierzy*/
#define matrixSize 8

enum Semafor { semaforKonsument = 0, semaforAtom, semaforProducent };
enum OperType { operacjaSuma = 1, operacjaRoznica, operacjaIloczyn };

struct task {
	enum OperType op;
	int a[matrixSize][matrixSize];
	int b[matrixSize][matrixSize];
};
#define shmSize (prodCnt*sizeof(struct task) + 2*sizeof(int))


extern key_t memKey;
extern key_t semKey;

void printTaskDetails(struct task*);


#endif /* COMMON_H_ */
