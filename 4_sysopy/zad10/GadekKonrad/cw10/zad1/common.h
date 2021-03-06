#ifndef _KPFP_COMMON
#define _KPFP_COMMON

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <poll.h>

#define myerror(str,myerrno) {\
                        fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
                        exit(myerrno);\
                }
#define myerrorpth(str,myerrno) {\
                        fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
                        pthread_exit((void*)myerrno);\
                }
#define NEW(type) ((type*)malloc(sizeof(type)))
#define NEWTAB(n,type) ((type*)malloc(((uint)(n))*sizeof(type)))

#define MAXNAMELEN 1024
#define MAXBUFSIZE 8196
#define MAXCOMMANDSIZE 1024


typedef unsigned int uint;


struct kpfpMsg {
	char command[MAXCOMMANDSIZE];
	char buf[MAXBUFSIZE];
	char name[MAXNAMELEN];
};

struct kpfpClEntry {
	struct sockaddr *addrSo;
	socklen_t addrSi;
	char name[MAXNAMELEN];
	int mode;
};


#endif
