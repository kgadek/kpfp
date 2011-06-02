#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>

#define CLNAMELEN 20
#define CLMSGLEN 300
#define KSNAME "KolejkaNaKasprowySzczyt"
#define KKNAME "KolejkaNaKasprowyKotlet"

#define myerror(str,myerrno) {\
			fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
			exit(myerrno);\
		}


typedef unsigned long ulong;
typedef unsigned int uint;


typedef struct {
	long type;
	char myNameIs[CLNAMELEN];
	pid_t myCard;
	char iWantToSay[CLMSGLEN];
} clMsg;

typedef struct {
	long type;
	char hisNameIs[CLNAMELEN];
	char heSaid[CLMSGLEN];
	uint LS;
	uint msgSent;
} svMsg;

void showClMsg(clMsg*, const char*);
void showSrvMsg(svMsg*, const char*);
void hr(void);
char getRandomChar(void);

#endif /* COMMON_H_ */
