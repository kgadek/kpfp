#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAXMSGSIZE 1024

int pisarz(int,char*);
int czytelnik(int);
void rozblokuj(int); /*ustawianie O_NONBLOCK na deskryptor pliku*/

int main(int argc, char **argv) {
	/* Program choruje na schizofrenię -- to czytelnik i pisarz w jednym */

	pid_t czytelnikPid, pisarzPid;
	int czytelnikStats, pisarzStats;

	int pipeFd[2];

	int c;
	int nonblockReadFlag = 0;
	int nonblockWriteFlag = 0;
	char *msg;

	msg = 0;
	while( (c = getopt(argc,argv,"rwm:")) != -1) {
		switch(c) {
			case 'r':
				nonblockReadFlag = 1;
				printf("main:\t\tO_NONBLOCK dla czytelnika\n");
				break;
			case 'w':
				nonblockWriteFlag = 1;
				printf("main:\t\tO_NONBLOCK dla pisarza\n");
				break;
			case 'm':
				msg = optarg;
				printf("main:\t\twiadomość: %s\n",msg);
				break;
			case '?':
				if(optopt == 'm')
					fprintf(stderr,"Brakuje wiadomości po -m!\n");
				else
					fprintf(stderr,"Nieprawidłowa opcja %c!\n",optopt);
				exit(5);
			default:
				abort();
		}
	}
	if(msg == 0) {
		msg = malloc(sizeof("Domyślna wiadomość")+1);
		strcpy(msg, "Domyślna wiadomość");
	}

	if(pipe(pipeFd) == -1) {
		fprintf(stderr,"Rurku, ten stół jest tak uperniczony piernikiem że nie da się tworzyć rury! %d\n",errno);
		exit(1);
	}

	if(nonblockReadFlag)
		rozblokuj(pipeFd[0]);
	if(nonblockWriteFlag)
		rozblokuj(pipeFd[1]);

	czytelnikPid = fork();
	if(czytelnikPid == -1) {
		fprintf(stderr,"Błąd tworzenia łyżki czytelnika! %d\n",errno);
		exit(2);
	} else if(!czytelnikPid) {
		close(pipeFd[1]);
		return czytelnik(pipeFd[0]);
	}

	pisarzPid = fork();
	if(pisarzPid == -1) {
		perror("Błąd tworzenia łyżki pisarza!\n");
		exit(3);
	} else if(!pisarzPid) {
		close(pipeFd[0]);
		/*jakby ktoś chciał przepiąć rurę na stdout:*/
		/*if(pipeFd[1] != STDOUT_FILENO) {
			if(dup2(pipeFd[1],STDOUT_FILENO) != STDOUT_FILENO) {
				fprintf(stderr, "Nie można podmienić stdout na pipeFd[1]! %d\n",errno);
				exit(4);
			}
			close(pipeFd[1]);
		}*/
		return pisarz(pipeFd[1],msg);
	}

	waitpid(czytelnikPid, &czytelnikStats, 0);
	printf("main:\t\tczytelnik returned with status = %d\n",czytelnikStats);

	waitpid(pisarzPid, &pisarzStats, 0);
	printf("main:\t\tpisarz returned with status = %d\n",pisarzStats);

	return 0;
}

int pisarz(int pout,char *msg) {
	sleep(1);
	if( write(pout,msg,strlen(msg)+1) == -1) {
		if(errno == EAGAIN) {
			fprintf(stderr,"pisarz:\t\trura pełna -- dajcie kreta (poczekam...)\n");
			sleep(1);
		} else {
			fprintf(stderr,"pisarz:\t\trura nieszczelna... ZALEWA NAS!\n");
			exit(7);
		}
	}
	return 0;
}

int czytelnik(int pin) {
	int n;
	char line[MAXMSGSIZE];
	printf("czytelnik:\ttu automatyczna sekretarka czytelnika, zostaw wiadomość po sygnale SIGINT... albo lepiej nie\n");
	do {
		n = read(pin,line,MAXMSGSIZE);
		if(n==-1)
			break;
		printf("czytelnik:\todebrałem %d-bajt%s: %s\n",n,
					n==1?"":(((1<n && n<5) || 21<n)?"y":"ów"),
					line);
	} while(1);
	if(n == -1 && errno == EAGAIN)
		printf("czytelnik:\tpisarz nic mi nie wysłał! już po deadline! czyżby nie żył?\n");
	else if(n==-1)
		printf("czytelnik:\trura zamilkła na wieki?... że niby... lol?\n");
	return 0;
}

void rozblokuj(int fd) {
	int oldf;
	oldf = fcntl(fd,F_GETFL);
	if(oldf == -1) {
		fprintf(stderr, "Nie mogę rozkminić fd=%d! %d\n",fd,errno);
		exit(6);
	}
	if( fcntl(fd, F_SETFL, oldf | O_NONBLOCK) == -1) {
		fprintf(stderr, "Nie mogę rozbloknąć fd=%d! %d\n",fd,errno);
		exit(7);
	}
}
