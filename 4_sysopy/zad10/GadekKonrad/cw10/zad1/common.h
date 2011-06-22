#ifndef _KPFP_COMMON
#define _KPFP_COMMON

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

#define MAXNAMELEN 1000
#define MAXBUFSIZE 512
#define MAXCOMMANDSIZE 32


typedef unsigned int uint;


struct kpfpMsg {
	char command[MAXCOMMANDSIZE];
	char buf[MAXBUFSIZE];
	char name[MAXNAMELEN];
};


#endif
