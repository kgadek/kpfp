#ifndef COMMON_H_
#define COMMON_H_

#include <sys/ipc.h>

#define myerror(str,myerrno) {\
			fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
			exit(myerrno);\
		}

typedef unsigned int uint;

#define prodCnt 5
const int semCnt = 3;
#define matrixSize 8


enum OperType { Suma, Roznica, Iloczyn };

struct task {
	enum OperType op;
	int a[matrixSize][matrixSize];
	int b[matrixSize][matrixSize];
};
const int shmSize = prodCnt*sizeof(struct task) + 2*sizeof(int);


key_t memKey = -1;
key_t semKey = -1;


#endif /* COMMON_H_ */
