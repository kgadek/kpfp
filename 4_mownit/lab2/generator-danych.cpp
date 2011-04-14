#include<cstdio>
#include<cstdlib>
#include<ctime>

int main(int argc, char *argv[]) {
	int tabsize, i;
	double **tab;
	double step;
	FILE *fd;

	if (argc != 3) {
		printf("Błędne wywołanie programu!\n"\
					"\t%s liczba-punktow plik-wynikowy\n",
					argv[0]);
		return 1;
	}

	srand(time(NULL));
	tabsize = atoi(argv[1]);
	step = 0;

	tab = (double **)malloc(tabsize * sizeof(double *));
	tab[0] = (double *)malloc(2 * sizeof(double));
	tab[0][0] = -3.0;
	tab[0][1] = (rand() * 2.0) / RAND_MAX * 1.0;

	for (i = 1; i < tabsize; i++) {
		step = (rand() * 1.0 + 1.0) / (RAND_MAX + 2.0);
		tab[i] = (double *)malloc(2 * sizeof(double));
		tab[i][0] = tab[i - 1][0] + step;
		tab[i][1] = ((rand() % 20) - 3.0) / 3.2;
	}

	fd = fopen(argv[2], "w");

	if (fd != NULL) {
		fprintf(fd, "%d\n", tabsize);
		for (i = 0; i < tabsize; i++)
			fprintf(fd, "%g %g\n", tab[i][0], tab[i][1]);
	} else {
		fprintf(stderr, "Nie można otworzyć pliku %s\n",argv[2]);
	}
	fclose(fd);
	return 0;
}

