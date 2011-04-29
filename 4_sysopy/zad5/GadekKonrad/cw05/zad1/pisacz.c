#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

char *napis; /*napis*/
char *inverted; /*odwrócony napis*/
unsigned int l; /*długość napisu*/


void invert(int sigid) {
	char *tmp;
	tmp = napis;
	napis = inverted;
	inverted = tmp;
}
void byebye(int sigid) {
	printf("Odebrano sygnał SIGINT... Żegnaj okrutny świecie!\n");
	free(napis);
	free(inverted);
	exit(0);
}


int main(int argc, char **argv) {
	unsigned int i; /*zmienna pomocnicza*/
	struct sigaction aInv;

	if(argc == 1) {
		fprintf(stderr,"Błędna liczba arugmentów!\n"\
					   "Wywołanie:\n"\
					   	   "\t%s napis\n",
				argv[0]);
		exit(1);
	}


	for(i=1,l=0;i<(unsigned int)argc;++i)
		l += strlen(argv[i]);
	l += (unsigned int)argc -1; /*dodatkowo spacje między wyrazami*/

	if(!(napis = (char*)malloc(l * sizeof(char))) || !(inverted = (char*)malloc(l * sizeof(char)))) {
		fprintf(stderr, "Błąd alokacji pamięci!\n");
		free(napis);
		exit(2);
	}

	strcpy(napis, argv[1]);
	for(i=2;i<(unsigned int)argc;++i) {
		strcat(napis, " ");
		strcat(napis, argv[i]);
	}

	for(i=0;i<l-1;++i)
		inverted[i] = napis[l-i-2];
	inverted[l-1] = 0;

	signal(SIGINT,byebye);
	aInv.sa_handler = invert;
	sigaction(SIGTSTP, &aInv, 0);

	while(1)
		printf("%s\n",napis);

	free(napis); /*tak dla zasady*/
	free(inverted);

	return 0;
}
