#include "common.h"

key_t memKey = -1;
key_t semKey = -1;

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

void printArray(FILE *out, int a[matrixSize][matrixSize]) {
	int i, j;
	fprintf(out,"(");
	for(i=0;i<matrixSize;++i) {
		fprintf(out,"(");
		for(j=0;j<matrixSize-1;++j)
			fprintf(out,"%d ",a[i][j]);
		fprintf(out, "%d)",a[i][matrixSize-1]);
	}
	fprintf(out,")\n");
}
