#define _GNU_SOURCE
#include "common.h"
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


void *mem = NULL;
int *imem = NULL;
int memCreated = 0;
int memId = -1;
int semId = -1;
int* prodCurrCnt;

int main(int argc, char **argv) {
	int i;
	int tmp;
	struct task shmTasks;
	struct sembuf semUnblock[2], semBlock[2];

	srand((uint)time(0));

	if(memKey == -1)
		memKey = ftok(".",811);

	memId = shmget(memKey, shmSize, IPC_CREAT | IPC_EXCL | 0755); /*try create*/
	if(memId == -1) {
		if(errno != EEXIST)
			myerror("Błąd shmget(3p)!",1);
		memId = shmget(memKey, prodCnt, 0); /*try get*/
		if(memId == -1)
			myerror("Błąd shmget(3p)",2);
		((int*)mem)[0] = 0;
	}

	mem = shmat(memId, 0, 0);
	if(mem == (void*)-1)
		myerror("Błąd shmat(3p)!",3);

	if(semKey == -1)
		semKey = ftok(".",812);

	semId = semget(semKey, semCnt, IPC_CREAT | IPC_EXCL | 0755);
	if(semId == -1) {
		if(errno != EEXIST)
			myerror("Błąd semget(3p)!",4);
		semId = semget(semKey, semCnt, 0);
		if(semId == -1)
			myerror("Błąd semget(3p)!",5);
	} else {
		for(i=0;i<2;++i)
			if(semctl(semId, i, SETVAL, i) == -1) /*blokuj sem0 (konsumentów), odblokuj sem1 (producentów)*/
				myerror("Błąd semctl(3p)!",6);
		if(semctl(semId, 2, SETVAL, prodCnt) == -1)
			myerror("Błąd semctl(3p)!",7);
	}

	imem = (int*)mem;
	shmTasks = (task*)(mem+2*sizeof(int));
	prodCurrCnt = imem;

	semBlock[0].sem_num = 1; /*blokowanie*/
	semBlock[1].sem_num = 2;
	semBlock[0].sem_op = semBlock[1].sem_op = -1;

	semUnblock[0].sem_num = 0; /*odblokowywanie */
	semUnblock[1].sem_num = 1;
	semUnblock[0].sem_op = semUnblock[1].sem_op = 1;

	for(i=0;i<2;++i)
		semBlock[i].sem_flg = semUnblock[i].sem_flg = 0;

	for(;;) { /*producentujemy*/
		tmp = semop(semId, semBlock, 2);
		if(tmp == -1)
			myerror("Błąd semop(3p)!",8);
	}

	return 0;
}
