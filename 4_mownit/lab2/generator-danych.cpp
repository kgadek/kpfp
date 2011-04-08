#include<cstdio>
#include<cstdlib>
#include<ctime>

int main(int argc, char *argv[]) {
	int a, i;
	double **tab;
	double krok;
	FILE *plik;

	if (argc != 3) {
		printf("Bledne wywolanie programu\n");
		return 1;
	}

	a = atoi(argv[1]);
	krok = 0;
	srand(time(NULL));

	tab = (double **)malloc(a * sizeof(double *));

	tab[0] = (double *)malloc(2 * sizeof(double));
	tab[0][0] = -3.0;
	tab[0][1] = (rand() * 2.0) / RAND_MAX * 1.0;

	for (i = 1; i < a; i++) {
		krok = (rand() * 1.0 + 1.0) / (RAND_MAX + 2.0);
		tab[i] = (double *)malloc(2 * sizeof(double));
		tab[i][0] = tab[i - 1][0] + krok;
		tab[i][1] = ((rand() % 20) - 3.0) / 3.2;
	}
	plik = fopen(argv[2], "w");
	fprintf(plik, "%d\n", a);
	if (plik != NULL) {
		for (i = 0; i < a; i++)
			fprintf(plik, "%g %g\n", tab[i][0], tab[i][1]);
	}
	fclose(plik);
	return 0;
}

