#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAXMSGLEN 1024
#define MAXPIDLEN 10

void myErr(int,const char*);

int main() {
	char *msg;
	char *mypid;
	int n;

	msg = malloc(MAXMSGLEN);
	mypid = malloc(MAXPIDLEN);
	sprintf(mypid," #%d",getpid());

	write(STDOUT_FILENO, mypid, strlen(mypid));
	write(STDOUT_FILENO, "\n", 1);
	write(STDERR_FILENO, "wypisalem\n",sizeof("wypisalem\n"));

	while(1) {
		n = read(STDIN_FILENO, msg, MAXMSGLEN);
		if(n == -1)
			myErr(3,"Błąd odczytu z stdin");
		if(!n)
			exit(0);

		if(n>0)
			msg[n-1] = 0; /*usuwamy \n z końca wiersza*/
		if(!strstr(msg,mypid)) { /*nie ma na liście*/
			strcat(msg,mypid);
			write(STDOUT_FILENO, msg, strlen(msg));
			write(STDOUT_FILENO, "\n", 1);
		} else { /*jest na liście*/
			write(STDERR_FILENO, msg, strlen(msg));
			write(STDERR_FILENO, "\n", 3);
		}
	}

	return 0;
}


void myErr(int code, const char *msg) {
	fprintf(stderr,"%s\nError code: %d\n",msg,errno);
	exit(code);
}
