#define _GNU_SOURCE
#include "common.h"

void *mem = NULL;
int *imem = NULL;
int memCreated = 0;
int memId = -1;
int semId = -1;
int* konsCurrCnt;
int rimuwEshaemAwterEgzit = 0;
int rimuwEseemAwterEgzit = 0;
FILE *fp = NULL;

void myatexit(void);
void mysighandler(int);
void doSuma(int[matrixSize][matrixSize], int[matrixSize][matrixSize], int[matrixSize][matrixSize]);
void doRoznica(int[matrixSize][matrixSize], int[matrixSize][matrixSize], int[matrixSize][matrixSize]);
void doIloczyn(int[matrixSize][matrixSize], int[matrixSize][matrixSize], int[matrixSize][matrixSize]);

int main(int argc, char *argv[]) {
	int i;
	/*int j;*/
	int tmp;
	struct task *shmTasks;
	struct sembuf semUnblock[2];
	struct sembuf semBlock[2];
	struct task newTask;
	int res[matrixSize][matrixSize];
	void (*operacja[4])(int[matrixSize][matrixSize],int[matrixSize][matrixSize],int[matrixSize][matrixSize]) = {doSuma, doSuma, doRoznica, doIloczyn};
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
	konsCurrCnt = imem+1;

	semBlock[0].sem_num = semaforKonsument; /*blokowanie konsumentów (teraz producenci) + gwarancja atomiczności*/
	semBlock[1].sem_num = semaforAtom;
	semBlock[0].sem_op = semBlock[1].sem_op = -1;

	semUnblock[0].sem_num = semaforProducent; /*odblokowywanie producentów (bo teraz oni rządzą) + zdjęcie gwarancji atomiczności*/
	semUnblock[1].sem_num = semaforAtom;
	semUnblock[0].sem_op = semUnblock[1].sem_op = 1;

	semBlock[0].sem_flg = semUnblock[0].sem_flg = \
			semBlock[1].sem_flg = semUnblock[1].sem_flg = 0;

	for(;;) { /*konsumentujemy*/

#if DEBUGMODE
		printf("\n------------------------------\n");
#endif

		tmp = semop(semId, semBlock, 2); /*blokujemy producentów + atomiczność*/
		if(tmp == -1)
			myerror("Błąd semop(3p)!",8);

#if DEBUGMODE
		printf(" [<-%d] ",imem[1]);
#endif
		newTask = shmTasks[imem[1]];
#if DEBUGMODE
		printf("[OK]\n");
		printTaskDetails(&newTask);
#endif
		imem[1] = (imem[1]+1)%prodAndKonsCnt;

		operacja[newTask.op](newTask.a, newTask.b, res);
		fprintf(fp,"A=\n");
		printArray(fp,newTask.a);
		fprintf(fp,"B=\n");
		printArray(fp,newTask.b);
		fprintf(fp,"RES=\n");
		printArray(fp,res);
		printf("Policzono %s\n", newTask.op==operacjaSuma?"sumę":(newTask.op==operacjaIloczyn?"iloczyn":"różnicę"));

		tmp = semop(semId, semUnblock, 2); /*odblokowujemy producentów + rozatomiczność*/
		if(tmp == -1)
			myerror("Błąd semop(3p)!",9);
	}

	return 0;
}

void myatexit(void) {
	printf("Kłitam // egzituję!\n");

	if(fp != NULL) {
		fclose(fp);
		fp = NULL;
	}
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

void doSuma(int a[matrixSize][matrixSize], int b[matrixSize][matrixSize], int c[matrixSize][matrixSize]) {
	int i, j;
	for(i=0; i<matrixSize; ++i)
		for(j=0; j<matrixSize; ++j)
			c[i][j] = a[i][j] + b[i][j];
}
void doRoznica(int a[matrixSize][matrixSize], int b[matrixSize][matrixSize], int c[matrixSize][matrixSize]) {
	int i, j;
	for(i=0; i<matrixSize; ++i)
		for(j=0; j<matrixSize; ++j)
			c[i][j] = a[i][j] - b[i][j];
}
void doIloczyn(int a[matrixSize][matrixSize], int b[matrixSize][matrixSize], int c[matrixSize][matrixSize]) {
	int i,j,k;
	for(i=0; i<matrixSize; ++i)
		for(j=0; j<matrixSize; ++j) {
			c[i][j] = 0;
			for(k=0; k<matrixSize; ++k)
				c[i][j] += a[i][k]+b[k][j];
		}
}
