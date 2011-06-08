#include "common.h"

extern key_t memKey = -1;
extern key_t semKey = -1;

void printTaskDetails(struct task *t) {
	int i;
	int j;
	printf("Task:\n");
	printf("\toperacja = %d\n",t->op);
	printf("\ta = (");
	for(i=0; i<matrixSize; ++i) {
		printf("(");
		for(j=0; j<matrixSize-1; ++j)
			printf("%d, ",t->a[i][j]);
		printf("%d)", t->a[i][matrixSize-1]);
	}
	printf("\n");
	printf("\tb = (");
	for(i=0; i<matrixSize; ++i) {
		printf("(");
		for(j=0; j<matrixSize-1; ++j)
			printf("%d, ",t->b[i][j]);
		printf("%d)", t->b[i][matrixSize-1]);
	}
	printf("\n");
}
