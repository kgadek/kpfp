#ifndef COMMON_H_
#define COMMON_H_

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define DEBUGMODE 1
#define myerror(str,myerrno) {\
			fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
			exit(myerrno);\
		}
#define SHMNAME "/Zad82Shm"
#define PISSEMNAME "/Zad82SemPis"
#define CZYTSEMNAME "/Zad82SemCzyt"
#define CZYTCNT 10

typedef unsigned int uint;

enum Semafor { semaforKonsument = 0, semaforAtom, semaforProducent };
enum OperType { operacjaSuma = 1, operacjaRoznica, operacjaIloczyn };


extern key_t memKey;
extern key_t semKey;

void openSem(sem_t **,const char *,int,int *P);


#endif /* COMMON_H_ */
