#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>

int binsearch(int, int*, int);
int testit();

int *A;

int main() {
	int i;
	struct tms sta_tms, sto_tms;
	clock_t start, stop;

	srand(time(0));

	A = malloc(32 * sizeof(int));

	for(i=0;i<32;++i)
		A[i] = i;

	start = times(&sta_tms);
	i = testit();
	stop = times(&sto_tms);

	printf("Czas %d: %jd\n", i, (stop-start));

	free(A);
	return 0;
}

int testit() {
	int max = 10000;
	int r = 0;
	int i;
	int j;
	int k;
	for(i=0;i<max;++i) {
		j = rand()%32;
		for(k = 0; k<max; ++k)
			r |= binsearch(j, A, 32);
	}
	return r;
}

int binsearch(int K, int *vect, int size) {
	int val;
	int i;
	int a = 0;
	--size;
	while(size >= a) {
		i = (a + size) / 2;
		val = vect[i];
		if(K >= val) {
			if(K == val)
				return i;
			a = i + 1;
		} else
			size = i - 1;
	}
	return -1;
}

