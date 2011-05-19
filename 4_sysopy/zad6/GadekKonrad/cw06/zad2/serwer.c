#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "common.h"


int main(int argc, char **argv) {
	char *n = 0;
	int tmp;
	int fd;
	packet p;
	int maxlen;
	mode_t fifoMode = 510; /* rwxrwxrw- */

	while( (tmp = getopt(argc, argv, "f:")) != -1) {
		switch(tmp) {
			case 'f':
				n = optarg;
				break;
			case '?':
				exit(5);
				break;
			default:
				abort();
		}
	}

	if(!n) {
		n = malloc(sizeof("domyslnaNazwaKolejkiFifoKtoraJestBardzoBardzoDlugaMoznabyNawetRzecZeZbytnio")+1);
		strcpy(n,"domyslnaNazwaKolejkiFifoKtoraJestBardzoBardzoDlugaMoznabyNawetRzecZeZbytnio");
	}


	if(mkfifo(n, fifoMode)) {
		if(errno == EEXIST)
			printf("//plik fifo \"%s\" już istnieje, co niecnie wykorzystuję...\n",n);
		else {
			fprintf(stderr,"Zonk mkfifo! %d\n", errno);
			exit(1);
		}
	}

	if( (fd = open(n,O_RDONLY)) == -1) {
		fprintf(stderr, "Błąd otwarcia pliku fifo! %d\n", errno);
		exit(2);
	}

	maxlen = 0;
	p.value = 0;

	while(1) {									/*czas sobie poserwerować*/
		tmp = read(fd, &p.length, sizeof(int));
		while(!tmp) {
			close(fd);
			if( (fd = open(n,O_RDONLY)) == -1) {
				fprintf(stderr, "Błąd rełołpen pliku fifo! %d\n", errno);
				exit(4);
			}
			tmp = read(fd, &p.length, sizeof(int));
		}
		if(tmp == -1) {
			fprintf(stderr, "Zonk czytania z fifo! %d\n", errno);
			exit(3);
		}
		if(p.length >= maxlen) {
			printf("// realokuję...\n");
			p.value = realloc(p.value, (size_t)(p.length + 1));
			maxlen = p.length + 1;
		}
		tmp = read(fd, p.value, (size_t)p.length);
		if(tmp == -1) {
			fprintf(stderr, "Zonk czytania stringa z fifo! %d\n", errno);
			exit(4);
		}
		p.value[tmp] = 0;
		printf("struct {\n    int length = %d;\n    char value[%d] = \"%s\";\n}\n\n\n",
				p.length, p.length, p.value);
	}

	return 0;
}
