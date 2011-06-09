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
#define SHMNAME "/4SOZZZ82Shm"
#define PISSEMNAME "/4SOZZZ82SemPis"
#define CZYTSEMNAME "/4SOZZZ82SemCzyt"
#define CZYTCNT 4

typedef unsigned int uint;

void openSem(sem_t **,const char *,int,int *P);


#endif /* COMMON_H_ */
