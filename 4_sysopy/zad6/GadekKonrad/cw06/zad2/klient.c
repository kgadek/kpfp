#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"
#include <errno.h>


int main(int argc, char **argv) {
	int tmp;
	char *n = 0; /*fifo name*/
	char c = 'a'; /*msg char*/
	char *m; /*msg buffer*/
	int l = 0; /*msg length*/
	int fd = -1; /*fifo file descr.*/

	while( (tmp = getopt(argc, argv, "f:l:c:")) != -1) {
		switch(tmp) {
			case 'f':
				n = optarg;
				break;
			case 'c':
				c = optarg[0];
				break;
			case 'l':
				l = atoi(optarg);
				break;
			case '?':
				break;
			default:
				abort();
		}
	}

	if(!n) {
		n = malloc(sizeof("domyslnaNazwaKolejkiFifoKtoraJestBardzoBardzoDlugaMoznabyNawetRzecZeZbytnio")+1);
		strcpy(n,"domyslnaNazwaKolejkiFifoKtoraJestBardzoBardzoDlugaMoznabyNawetRzecZeZbytnio");
	}

	fd = open(n,O_WRONLY);
	if(fd == -1) {
		fprintf(stderr, "Błąd otwarcia fifo! Czy chcesz otworzyć PES? %d\n", errno);
		exit(1);
	}

	m = malloc((size_t)l);
	for(tmp=0; tmp<l;++tmp)
		m[tmp] = c;

	write(fd, &l, sizeof(l));			/* żeby wysłać string trzeba wysłać go osobno poza strukturą... */
	write(fd, m, (size_t)l);			/* więc poza ,,ustaleniem wyglądu'' to tutaj ze struktury nie korzystamy */

	close(fd); /*poszło!*/

	return 0;
}
