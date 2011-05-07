#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>

#define arrlen(arr,type) (sizeof(arr)/sizeof(type))

void lu();
void qr();
void qrpiv();
void showHelpAndExit(char *, int, const char *);

int main(int argc, char **argv) {
	int i;											/*licznik*/
	FILE *fp;										/*plik z danymi*/
	char *tmp;
	const char *methsS[] = {"lu", "qr", "qrpiv"};	/*stringi*/
	void (*methsF[])() = {lu, qr, qrpiv};			/*majtk... yyy funkcje*/

	if(argc != 3) showHelpAndExit(argv[0],1,NULL);

	fp = fopen(argv[1],"r");
	if(!fp) {
		tmp = malloc(100*sizeof(char));
		snprintf(tmp,100,"Nie można otworzyć pliku %s\n",argv[1]);
		showHelpAndExit(argv[0],3,tmp);
	}

	for(i=0; i<(int)arrlen(methsS,const char*); ++i)	/*searchin'... seek and exec!*/
		if(! strcmp(argv[2],methsS[i])) {
			methsF[i]();
			break;
		}

	fclose(fp);

	if(i >= (int)arrlen(methsS,const char*))			/*noting' found, dude!*/
			showHelpAndExit(argv[0],2,NULL);

	return 0;
}


void showHelpAndExit(char *execname, int exitCode, const char *opt) {
    fprintf(stderr, "Błedne wywołanie programu!\n%s"\
							"\t%s plik metoda\t---\trozwiąż układ. Metody:\n"\
							"\t\t\t\tlu\n"\
							"\t\t\t\tqr\n"\
							"\t\t\t\tqrpiv\n",
						opt?opt:"",execname);
    exit(exitCode);
    free(opt);
}

void lu() {
	printf("lu\n");
}
void qr() {
	printf("qr\n");
}
void qrpiv() {
	printf("qrpiv\n");
}

