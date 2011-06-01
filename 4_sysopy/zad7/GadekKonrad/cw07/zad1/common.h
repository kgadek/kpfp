#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define CLNAMELEN 20
#define CLMSGLEN 300

#define myerror(str,myerrno) {\
			fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
			exit(myerrno);\
		}


typedef unsigned long ulong;
typedef unsigned int uint;


typedef struct {
	long type;
	int myQueueNum;
	char myNameIs[CLNAMELEN];
	char iWantToSay[CLMSGLEN];
} clMsg;

typedef struct {
	long type;
	char hisNameIs[CLNAMELEN];
	char heSaid[CLMSGLEN];
	msgqnum_t msgInQueue;
	ulong bytesInQueue;
	msglen_t maxBytesInQueue;
	pid_t pidReader;
	pid_t pidSender;
	time_t lastRead;
	time_t lastSend;
} svMsg;

#endif /* COMMON_H_ */
