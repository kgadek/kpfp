#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>

#define arrlen(arr,type) (sizeof(arr)/sizeof(type))
typedef unsigned int uint;

void lu(uint,gsl_matrix*,gsl_vector*,gsl_vector*);
void qr(uint,gsl_matrix*,gsl_vector*,gsl_vector*);
void qrpiv(uint,gsl_matrix*,gsl_vector*,gsl_vector*);
void sing(uint,gsl_matrix*,gsl_vector*,gsl_vector*);
void hous(uint,gsl_matrix*,gsl_vector*,gsl_vector*);

void showHelpAndExit(char *, int, char *, const char **, uint);


int main(int argc, char **argv) {
	uint i;												/*liczniki*/
	uint j;
	uint n;												/*rozmiar macierzy kwadratowej*/
	FILE *fp;											/*plik z danymi*/
	char *tmp;
	gsl_matrix *A;										/*wspólne dla podprogramów*/
	gsl_vector *b;
	gsl_vector *X;

	const char *methsS[] = \
			{"lu", "qr", "qrpiv", "sing", "hous"};
														/*stringi*/
	void (*methsF[])(uint,gsl_matrix*,gsl_vector*,gsl_vector*) = \
			{lu, qr, qrpiv, sing, hous};
														/*majtk... yyy funkcje*/


	if(argc == 4 && !strcmp(argv[1],"-g")) {			/*generator*/
		n = (uint)strtol(argv[3],&tmp,10);					/*wczytaj n*/
		if(tmp == argv[3])									/*zonk strtol(...)*/
			showHelpAndExit(argv[0],5,NULL,methsS,arrlen(methsS,const char*));

		srand((uint)time(NULL));									/*zainicjuj generator liczb pseudolosowych*/
		fp = fopen(argv[2],"w");							/*otwórz plik*/
		fprintf(fp,"%u\n",n);								/*zapisz rozmiar*/

		for(i=0;i<n;++i) {									/*zapisz macierz A*/
			for(j=0;j<n;++j)
				fprintf(fp,"%d ",rand()-RAND_MAX/2);
			fprintf(fp,"\n");
		}

		for(i=0;i<n;++i)									/*zapisz wektor b*/
			fprintf(fp,"%d\n",rand()-RAND_MAX/2);
		fprintf(fp,"\n");

		fclose(fp);											/*to już jest koniec...*/
		exit(0);											/*...nie ma już nic*/
	}


	if(argc != 3)
		showHelpAndExit(argv[0],1,NULL,methsS,arrlen(methsS,const char*));

	fp = fopen(argv[1],"r");									/*powiedz przyjacielu i otwórz*/
	if(!fp) {
		tmp = malloc(100*sizeof(char));								/*you know it's sad but true!*/
		snprintf(tmp,100,"Nie można otworzyć pliku %s\n",argv[1]);
		showHelpAndExit(argv[0],3,tmp,methsS,arrlen(methsS,const char*));
	}

	fscanf(fp,"%u",&n);											/*powiedz przyjacielu i wczytaj*/
	A = gsl_matrix_alloc(n,n);									/*powiedz przyjacielu i zaalokuj*/
	b = gsl_vector_alloc(n);
	X = gsl_vector_alloc(n);
	if(gsl_matrix_fscanf(fp,A) || gsl_vector_fscanf(fp,b)) {	/*powiedz przyjacielu i wczytaj*/
		gsl_vector_free(X);											/*powiedz przyjacielu i... przepadnij?*/
		gsl_vector_free(b);
		gsl_matrix_free(A);
		fclose(fp);
		showHelpAndExit(argv[0],4,NULL,methsS,arrlen(methsS,const char*));
	}
	fclose(fp);

	for(i=0; i<(uint)arrlen(methsS,const char*); ++i)			/*searchin'... seek and exec!*/
		if(! strcmp(argv[2],methsS[i])) {
			methsF[i](n,A,b,X);
			break;
		}

	if(i >= (int)arrlen(methsS,const char*))					/*noting' found, dude!*/
			showHelpAndExit(argv[0],2,NULL,methsS,arrlen(methsS,const char*));
	else														/*report on situation, sir!*/
		gsl_vector_fprintf(stdout,X,"%.3f");

	return 0;
}


void showHelpAndExit(char *execname, int exitCode, char *opt, const char **methsS, uint methsN) {
    fprintf(stderr, "Błedne wywołanie programu! %s"\
    				"Wywołanie programu:\n"
							"\t%s plik metoda     --- rozwiąż układ równań liniowych\n\t\t\t\t\tpostaci Ax = b.\n"\
							"\t%s -g plik rozmiar --- generuje losowy układ Ax = b\n\t\t\t\t\to zadanym rozmiarze.\n"\
							"Dostępne metody:\n",
					opt?opt:"",execname,execname);
    while(methsN--)
    	fprintf(stderr,"\t%s\n",methsS[methsN]);
    exit(exitCode);
    free(opt);
}

void lu(uint n,gsl_matrix *A,gsl_vector *b,gsl_vector* X) {
	gsl_permutation *P;
	int s;

	P = gsl_permutation_alloc(n);
	gsl_linalg_LU_decomp(A,P,&s);
	gsl_linalg_LU_solve(A,P,b,X);

	gsl_permutation_free(P);
}
void qr(uint n,gsl_matrix *A,gsl_vector *b,gsl_vector* X) {
	gsl_vector *T;

	T = gsl_vector_alloc(n);
	gsl_linalg_QR_decomp(A,T);
	gsl_linalg_QR_solve(A,T,b,X);

	gsl_vector_free(T);
}
void qrpiv(uint n,gsl_matrix *A,gsl_vector *b,gsl_vector* X) {
	int s;
	gsl_vector *T;
	gsl_vector *no;
	gsl_permutation *P;

	T = gsl_vector_alloc(n);
	no = gsl_vector_alloc(n);
	P = gsl_permutation_alloc(n);

	gsl_linalg_QRPT_decomp(A,T,P,&s,no);
	gsl_linalg_QRPT_solve(A,T,P,b,X);

	gsl_permutation_free(P);
	gsl_vector_free(no);
	gsl_vector_free(T);
}

void sing(uint n,gsl_matrix *A,gsl_vector *b,gsl_vector* X) {
	gsl_matrix *V;
	gsl_vector *S;
	gsl_vector *w;

	V = gsl_matrix_alloc(n,n);
	S = gsl_vector_alloc(n);
	w = gsl_vector_alloc(n);

	gsl_linalg_SV_decomp(A,V,S,w);
	gsl_linalg_SV_solve(A,V,S,b,X);

	gsl_vector_free(w);
	gsl_vector_free(S);
	gsl_matrix_free(V);
}

void hous(uint n,gsl_matrix *A,gsl_vector *b,gsl_vector* X) {
	gsl_linalg_HH_solve(A,b,X);
	n=n;	/*yeah, I know I didn't use "n", you stupid gcc!*/
}
