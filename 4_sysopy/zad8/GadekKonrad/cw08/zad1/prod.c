#define _GNU_SOURCE
#include "common.h"

void *mem = NULL;
int *imem = NULL;
int memCreated = 0;
int memId = -1;
int semId = -1;
int* prodCurrCnt;
int rimuwEshaemAwterEgzit = 0;
int rimuwEseemAwterEgzit = 0;

void myatexit(void);
void mysighandler(int);

int main() {
	int i; /*liczniki*/
	int j;
	int tmp;
	struct task *shmTasks;
	struct sembuf semUnblock[2];
	struct sembuf semBlock[2];
	struct task newTask;
	/* semafory:
	 * 0 -- blokada konsumentów
	 * 1 -- blokada gwarantująca atomiczność operacji na wspólnej pamięci
	 * 2 -- blokada producentów*/

	srand((uint)time(0));
	if(memKey == -1)
		memKey = ftok(".",811);
	tmp = atexit(myatexit);

	memId = shmget(memKey, (size_t)shmSize, IPC_CREAT | IPC_EXCL | 0755); /*try create*/
	if(memId == -1) {
		if(errno != EEXIST) {
			myerror("Błąd shmget(3p)!",1);
		}
		memId = shmget(memKey, prodAndKonsCnt, 0); /*try get*/
		if(memId == -1) {
			myerror("Błąd shmget(3p)",2);
		}
	} else {
		rimuwEshaemAwterEgzit = 1;
	}

	mem = shmat(memId, 0, 0);
	if(mem == (void*)-1) {
		myerror("Błąd shmat(3p)!",3);
	} else if(rimuwEshaemAwterEgzit) {
		((int*)mem)[0] = 0; /*jeśli utworzyliśmy -- wyzerujmy licznik klientów i producentów*/
		((int*)mem)[1] = 0;
	}

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
		rimuwEseemAwterEgzit = 1;
		for(i=0;i<2;++i)
			if(semctl(semId, i, SETVAL, i) == -1) /*blokuj sem0 (konsumentów), odblokuj sem1 (licznik)*/
				myerror("Błąd semctl(3p)!",6);
		if(semctl(semId, 2, SETVAL, prodAndKonsCnt) == -1) /*odblokuj sem2 (producentów)*/
			myerror("Błąd semctl(3p)!",7);
	}

	imem = (int*)mem;
	shmTasks = (struct task*)((int*)mem+2*sizeof(int));
	prodCurrCnt = imem;

	semBlock[0].sem_num = semaforAtom; /*blokowanie producentów (teraz klienci) + gwarancja atomiczności*/
	semBlock[1].sem_num = semaforProducent;
	semBlock[0].sem_op = semBlock[1].sem_op = -1;

	semUnblock[0].sem_num = semaforKonsument; /*odblokowywanie klientów (bo teraz oni rządzą) + zdjęcie gwarancji atomiczności*/
	semUnblock[1].sem_num = semaforAtom;
	semUnblock[0].sem_op = semUnblock[1].sem_op = 1;

	semBlock[0].sem_flg = semUnblock[0].sem_flg = \
			semBlock[1].sem_flg = semUnblock[1].sem_flg = 0;

	for(;;) { /*producentujemy*/

		newTask.op = rand()%(operacjaIloczyn-operacjaSuma+1)+operacjaSuma;

		for(i=0; i<matrixSize; ++i) {
			for(j=0; j<matrixSize; ++j) {
				newTask.a[i][j] = rand()%10;
				newTask.b[i][j] = rand()%10;
			}
		}

#if DEBUGMODE
		printf("\n------------------------------\nUmieszczam task:\n\top=%d\n\ta= (",newTask.op);
		printTaskDetails(&newTask);
		printf("\n\tDodawanie... ");
#endif

		tmp = semop(semId, semBlock, 2); /*blokujemy producentów + atomiczność*/
		if(tmp == -1)
			myerror("Błąd semop(3p)!",8);
#if DEBUGMODE
		printf(" [xx]");
#endif

#if DEBUGMODE
		printf(" [%d]",imem[0]);
#endif
		shmTasks[imem[0]] = newTask;
#if DEBUGMODE
		printf(" [OK]");
#endif

		tmp = semop(semId, semUnblock, 2); /*odblokowujemy producentów + rozatomiczność*/
		imem[0] = (imem[0]+1)%prodAndKonsCnt;
		if(tmp == -1)
			myerror("Błąd semop(3p)!",9);
#if DEBUGMODE
		printf(" [++]\n");
#else
		printf("Umieściłem zadanie w pamięci\n");
#endif
	}

	return 0; /*never called*/
}

void myatexit(void) {
	printf("Kłitam // egzituję\n");
	if(mem != (void*)-1) {
		if(shmdt(mem) == -1)
			fprintf(stderr, "Błąd shmdt(3p)! myerror=%d errno=%d\n",10,errno);
		if(rimuwEshaemAwterEgzit && shmctl(memId, IPC_RMID, 0) == -1)
			fprintf(stderr, "Błąd shmctl(3p)! myerror=%d errno=%d\n",12,errno);
	}
	if(semId != -1 && rimuwEseemAwterEgzit && semctl(semId, IPC_RMID, 0) == -1)
		fprintf(stderr, "Błąd semctl(3p)! myerror=%d errno=%d\n",11,errno);

}

void mysighandler(int sigid) {
	printf("\n\n--------------------\n\nZabijasz mnie %d? Ale %d?!...\n",sigid, sigid);
	exit(0);
}
