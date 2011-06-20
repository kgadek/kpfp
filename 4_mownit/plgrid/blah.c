#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double fun(double x) {
	return (x-5)*(x-5);
}

struct indiv {
	double x;
	double fit;
};

int main(int argc, char *argv[]) {
	int rank, size, i, best, besti;
	double bestd;
	srand(time(0));
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	struct indiv inmsg, outmsg;
	int rc;
	MPI_Status Stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &size);

	struct indiv *pop = (struct indiv*)malloc(2*size*sizeof(struct indiv));

	printf("It's me, %d!\n", rank);
	if(rank==0) printf("Total: %d nodes\n",size);

	bestd = 1000000;
	besti = 0;
	for(i=rank;i<2*(rank+1)-1;++i) {
	   pop[i].x = (double)(rand() % 500)*0.01;
	   pop[i].fit = fun(pop[i].x);
	   if(pop[i].fit < bestd) {
		   bestd = pop[i].fit;
		   besti = i;
		   printf("%d's new best: fit=%f with x=%f at %d\n",rank,bestd,pop[i].x,i);
	   }
	};

	if(rank == 0) {
		printf("Me haz %f\n", bestd);
		for(i=1;i<size;++i) {
			printf("Wait for %d...", i);
			rc = MPI_Recv(&inmsg, sizeof(struct indiv), MPI_CHAR, i, 1, MPI_COMM_WORLD, &Stat);
			printf("OK, got: %f\n",inmsg.x);
			if(fun(bestd)>inmsg.fit)
				bestd=inmsg.x;
		}
		printf("Global best: %f\n", bestd);
	} else {
		MPI_Send(pop+besti, sizeof(struct indiv), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

