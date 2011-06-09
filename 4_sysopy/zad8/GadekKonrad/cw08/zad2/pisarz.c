#define _GNU_SOURCE
#include "common.h"


int *mem = NULL;
int memId;
int rimuwEshaemAwterEgzit = 0;
int rimuwEseemPisAwterEgzit = 0;
int rimuwEseemCzytAwterEgzit = 0;
sem_t *semCzyt;
sem_t *semPis;

void myatexit(void);
void mysighandler(int);
void openSem(sem_t **,const char *,int,int *);

int main() {
	int i; /*liczniki*/
	int tmp;

	srand((uint)time(0));
	tmp = atexit(myatexit);
	tmp = (int)signal(SIGINT, mysighandler);
	if(tmp == (int)SIG_ERR)
		myerror("Błąd signal(3)!",10);

	memId = shm_open(SHMNAME, O_RDWR | O_CREAT | O_EXCL, 0755); /*try create*/
	if(memId == -1) {
		if(errno != EEXIST) {
			myerror("Błąd shm_open(3p)!",1);
		}
		memId = shm_open("/Zad82Mem", O_RDWR, 0); /*try get*/
		if(memId == -1) {
			myerror("Błąd shm_open(3p)",2);
		}
	} else {
		rimuwEshaemAwterEgzit = 1;
	}
	tmp = ftruncate(memId, sizeof(mem));
	if(tmp == -1)
		myerror("Błąd ftruncate(3p)!",4);
	mem = (int*)mmap(NULL, 100*sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, memId, 0);
	if(mem == MAP_FAILED)
		myerror("Błąd mmap(3p)!",5);
	if(rimuwEseemCzytAwterEgzit)
		for(i=0; i<100; ++i)
			mem[i] = rand()%100;

	openSem(&semPis, PISSEMNAME, 1, &rimuwEseemPisAwterEgzit);
	openSem(&semCzyt, CZYTSEMNAME, CZYTCNT, &rimuwEseemCzytAwterEgzit);

	for(;;) { /*pisarzujemy*/

		printf("Jesteśmy grzeczni, czekamy na innych pisarzy...");
		tmp = sem_wait(semPis);
		if(tmp == -1)
			myerror("Błąd sem_wait(3p)",6);
		printf(" oraz czytelników...\n");
		for(i=0; i<CZYTCNT; ++i) {
			tmp  = sem_wait(semCzyt);
			if(tmp == -1)
				myerror("Błąd sem_wait(3p)",7);
		}
		printf("A teraz grzecznie piszemy po książce jak na dobrego chochlika przystało ;]\n");
		mem[rand()%100]=rand()%100;
		printf("Ktoś idzie?! Uciekam przed czytelnikami ...");
		for(i=0; i<CZYTCNT; ++i) {
			tmp = sem_post(semCzyt);
			if(tmp == -1)
				myerror("Błąd sem_post(3p)",8);
		}
		printf(" i pisarzami!\n");
		tmp = sem_post(semPis);
		if(tmp == -1)
			myerror("Błąd sem_post(3p)",9);

	}

	return 0; /*never called*/
}

void myatexit(void) {
	printf("Kłitam // egzituję\n");
	if(rimuwEseemCzytAwterEgzit && sem_unlink(CZYTSEMNAME) == -1)
		fprintf(stderr, "Buba z shm_unlink(3p)!");
	if(rimuwEseemPisAwterEgzit && sem_unlink(PISSEMNAME) == -1)
		fprintf(stderr, "Buba z shm_unlink(3p)!");
	if(rimuwEshaemAwterEgzit &&  shm_unlink(SHMNAME) == -1)
		fprintf(stderr, "Buba z shm_unlink(3p)!");
	if(mem != MAP_FAILED)
		munmap(mem, 100);

}

void mysighandler(int sigid) {
	printf("\n\n--------------------\n\nZabijasz mnie %d? Ale %d?!...\n",sigid, sigid);
	exit(0);
}


