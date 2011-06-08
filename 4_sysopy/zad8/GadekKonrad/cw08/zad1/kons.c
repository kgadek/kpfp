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
FILE *fp;

void myatexit(void);
void mysighandler(int);

int main(int argc, char *argv[]) {
	int i;
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
	if(argc != 2)
		myerror("Błędna ilość argumentów (należy podać plik logów)",1);
	fp =  fopen(argv[1],"a");
	if(fp == NULL)
		myerror("Błąd otwarcia podanego pliku do zapisu (dopisywania)",2);

	if(memKey == -1)
		memKey = ftok(".",811);
	/*tmp = atexit(myatexit);
	if(SIG_ERR != signal(SIGKILL, mysighandler))
		myerror("Błąd signal(3p)!",13);
	if(SIG_ERR != signal(SIGTERM, mysighandler))
		myerror("Błąd signal(3p)!",14);
	if(SIG_ERR != signal(SIGHUP, mysighandler))
		myerror("Błąd signal(3p)!",15);*/

	memId = shmget(memKey, (size_t)shmSize, IPC_CREAT | IPC_EXCL | 0755); /*try create*/
	if(memId == -1) {
		if(errno != EEXIST)
			myerror("Błąd shmget(3p)!",1);
		memId = shmget(memKey, prodCnt, 0); /*try get*/
		if(memId == -1)
			myerror("Błąd shmget(3p)",2);
		((int*)mem)[0] = ((int*)mem)[1] = 0; /*jeśli utworzyliśmy -- wyzerujmy licznik klientów i producentów*/
		rimuwEshaemAwterEgzit = 1;
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
		rimuwEseemAwterEgzit = 1;
	} else {
		for(i=0;i<2;++i)
			if(semctl(semId, i, SETVAL, i) == -1) /*blokuj sem0 (konsumentów), odblokuj sem1 (licznik)*/
				myerror("Błąd semctl(3p)!",6);
		if(semctl(semId, 2, SETVAL, prodCnt) == -1) /*odblokuj sem2 (producentów)*/
			myerror("Błąd semctl(3p)!",7);
	}

	imem = (int*)mem;
	shmTasks = (struct task*)((int*)mem+2*sizeof(int));
	prodCurrCnt = imem;

	semBlock[0].sem_num = semaforAtom; /*blokowanie konsumentów (teraz producenci) + gwarancja atomiczności*/
	semBlock[1].sem_num = semaforKonsument;
	semBlock[0].sem_op = semBlock[1].sem_op = -1;

	semUnblock[0].sem_num = semaforProducent; /*odblokowywanie producentów (bo teraz oni rządzą) + zdjęcie gwarancji atomiczności*/
	semUnblock[1].sem_num = semaforAtom;
	semUnblock[0].sem_op = semUnblock[1].sem_op = 1;

	semBlock[0].sem_flg = semUnblock[0].sem_flg = \
			semBlock[1].sem_flg = semUnblock[1].sem_flg = 0;

	for(;;) { /*konsumentujemy*/


		printf("\n------------------------------\n");
		printf("Czytam zadanie na dziś...");

		tmp = semop(semId, semBlock, 2); /*blokujemy producentów + atomiczność*/
		if(tmp == -1)
			myerror("Błąd semop(3p)!",8);

		imem[1] = (imem[1]+1)%prodCnt;
		printf(" [<-%d] ",imem[1]);
		newTask = shmTasks[imem[1]];
		printf("[OK]\n");
		printTaskDetails(&newTask);

		tmp = semop(semId, semBlock+1, 2); /*odblokowujemy producentów + rozatomiczność*/
		if(tmp == -1)
			myerror("Błąd semop(3p)!",9);
	}

	return 0;
}

void myatexit(void) {
	printf("Kłitam");
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
