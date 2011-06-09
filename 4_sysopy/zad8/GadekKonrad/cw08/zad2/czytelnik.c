#define _GNU_SOURCE
#include "common.h"

int rimuwEshaemAwterEgzit = 0;
int rimuwEseemCzytAwterEgzit = 0;
sem_t *semCzyt;
int memId;
int *mem;
enum operacja {maksimum, minimum, srednia};

void myatexit(void);
void mysighandler(int);
void doMaksimum(void);
void doMinimum(void);
void doSrednia(void);

int main(int argc, char *argv[]) {
	int tmp;
	enum operacja operacja;
	void (*operacje[3])(void) = {doMaksimum, doMinimum, doSrednia};

	srand((uint)time(0));
	if(argc != 2)
		myerror("Błędna ilość argumentów (należy podać: max, min, avg)",1);
	if(!strcmp("avg",argv[1]))
		operacja=srednia;
	else if(!strcmp("max",argv[1]))
		operacja=maksimum;
	else if(!strcmp("min",argv[1]))
		operacja=minimum;
	else {
		myerror("Błędny argument!",2);
	}

	tmp = atexit(myatexit);
	if(tmp != 0)
		myerror("Błąd atexit(3)!",3);
	tmp = (int)signal(SIGINT, mysighandler);
	if(tmp == (int)SIG_ERR)
		myerror("Błąd signal(3)!",4);

	memId = shm_open(SHMNAME, O_RDWR | O_CREAT | O_EXCL, 0755); /*try create*/
	if(memId == -1) {
		if(errno != EEXIST) {
			myerror("Błąd shmget(3p)!",5);
		}
		memId = shm_open(SHMNAME, O_RDWR, 0); /*try get*/
		if(memId == -1) {
			myerror("Błąd shmget(3p)",6);
		}
	} else {
		rimuwEshaemAwterEgzit = 1;
	}
	tmp = ftruncate(memId, sizeof(mem));
	if(tmp == -1)
		myerror("Błąd ftruncate(3p)!",7);
	mem = (int*)mmap(NULL, 100*sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, memId, 0);
	if(mem == MAP_FAILED)
		myerror("Błąd mmap(3p)!",8);

	openSem(&semCzyt, CZYTSEMNAME, CZYTCNT, &rimuwEseemCzytAwterEgzit);

	for(;;) { /*czytelnikujemy*/

		tmp = sem_wait(semCzyt);
		if(tmp == -1)
			myerror("Błąd sem_wait(3p)",9);

		operacje[operacja]();

		tmp = sem_post(semCzyt);
		if(tmp == -1)
			myerror("Błąd sem_post(3p)",10);
	}

	return 0;
}

void myatexit(void) {
	printf("Kłitam // egzituję!\n");

	if(rimuwEshaemAwterEgzit && shm_unlink(SHMNAME) == -1)
		fprintf(stderr,"Błąd shm_unlink(3p)\n");
	if(rimuwEseemCzytAwterEgzit && sem_unlink(CZYTSEMNAME) == -1)
		fprintf(stderr,"Błąd shm_unlink(3p)\n");
	if(mem != MAP_FAILED)
		munmap(mem, 100);
}

void mysighandler(int sigid) {
	printf("\n\n--------------------\n\nZabijasz mnie %d? Ale %d?!...\n",sigid, sigid);
	exit(0);
}

void doMaksimum(void) {
	int res = 0;
	int i = 1;
	for(; i<100; ++i)
		if(mem[i]>mem[res])
			res = i;
	printf("Maksimum wynosi %d\n",res);
}
void doMinimum(void) {
	int res = 0;
	int i = 1;
	for(; i<100; ++i)
		if(mem[i]<mem[res])
			res = i;
	printf("Minimum wynosi %d\n",res);
}
void doSrednia(void) {
	int sum = 0;
	int i = 0;
	for(; i<100; ++i)
		sum += mem[i];
	printf("Średnia: %.3f\n", ((double)sum)/((double)100.0));
}

