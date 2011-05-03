/*Zadanie 2

Napisz dwa programy: sender i receiver. Niech sender wysyła sygnał SIGUSR1 przy użyciu funkcji kill
lub sigqueue. Wybór jednej z tych funkcji powinien być dokonywany na podstawie parametru przekazanego
w argumencie wywołania programu. W przypadku zastosowania funkcji sigqueue w argumentach programu musi
wystąpić jeszcze jeden parametr, zawierający wartość (liczbę całkowitą), która będzie przesłana wraz z
sygnałem. Program receiver powinien odbierać przesłany sygnał, wypasując na ekranie odpowiedni
komunikat wraz z dodatkowymi informacjami o sygnale (rzeczywisty identyfikator użytkownika
uruchamiającego proces sender, PID procesu sender oraz przekazany czas użytkownika). W przypadku
przesyłania z sygnałem dodatkowej wartości, receiver również wypisuje ją na ekranie. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handler(int,siginfo_t *,void *);

int main() {
	struct sigaction a;

	sigfillset(&a.sa_mask);
	a.sa_flags = SA_SIGINFO;
	a.sa_sigaction = handler;
	sigaction(SIGUSR1,&a,0);

	while(1)
		pause();

	return 0;
}

void handler(int sigid,siginfo_t *si,void *cont) {
	printf("rcv");
	printf("\t+-[ SIGNAL RECEIVED: %d ]----------------------|\n",sigid);
	printf("\t|           | si_signo  | %20d |\n",si->si_signo);
	printf("\t|           | si_errno  | %20d |\n",si->si_errno);
	switch(si->si_code) {
		case SI_USER:		printf("\t|           | si_code   |     SI_USER    %5d |\n",si->si_code);
				break;
		case SI_KERNEL:		printf("\t|           | si_code   |     SI_KERNEL  %5d |\n",si->si_code);
				break;
		case SI_QUEUE:		printf("\t|           | si_code   |     SI_QUEUE   %5d |\n",si->si_code);
				break;
		case SI_TIMER:		printf("\t|           | si_code   |     SI_TIMER   %5d |\n",si->si_code);
				break;
		case SI_MESGQ:		printf("\t|           | si_code   |     SI_MESGQ   %5d |\n",si->si_code);
				break;
		case SI_ASYNCIO:	printf("\t|           | si_code   |     SI_ASYNCIO %5d |\n",si->si_code);
				break;
		case SI_SIGIO:		printf("\t|           | si_code   |     SI_SIGIO   %5d |\n",si->si_code);
				break;
		default: 			printf("\t|           | si_code   |     SI_ ???    %5d |\n",si->si_code);
	}
	printf("\t|           | si_pid    | %20d |\n",si->si_pid);
	printf("\t|           | si_uid    | %20d |\n",si->si_uid);
	printf("\t|           | si_status | %20d |\n",si->si_status);
	printf("\t|           | si_utime  | %20d |\n",si->si_utime);
	printf("\t| siginfo_t | si_stime  | %20d |\n",si->si_stime);
	printf("\t|           | si_value  | %20d |\n",si->si_value);
	printf("\t|           | si_int    | %20d |\n",si->si_int);
	printf("\t|           | si_ptr    | %20d |\n",si->si_ptr);
	printf("\t|           | si_addr   | %20d |\n",si->si_addr);
	printf("\t|           | si_band   | %20d |\n",si->si_band);
	printf("\t|           | si_fd     | %20d |\n",si->si_fd);
	printf("\t+-----------+-----------+----------------------|\n\n");

}
