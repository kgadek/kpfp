#include "common.h"


void openSem(sem_t **p,const char *str,int cnt,int *resCreatedP) {
	*p = sem_open(str, O_CREAT | O_EXCL, 0755, cnt);
	if(*p == SEM_FAILED) {
		if(errno != EEXIST)
			myerror("Błąd sem_open(3p)!",4);
		*p = sem_open(str, 0);
		if(*p == SEM_FAILED)
			myerror("Błąd sem_open(3p)!",5);
	} else
		*resCreatedP = 1;
}
