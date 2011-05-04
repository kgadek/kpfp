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
#include <time.h>


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
	printf("+-[ SIG RCVD: %d cont: %12d ]----------+\n",sigid,(int)cont);
	printf("|           | si_signo  | %20d |\n",si->si_signo);
	printf("|           | si_errno  | %20d |\n",si->si_errno);
	switch(si->si_code) {
		case SI_USER:		printf("|           | si_code   |    SI_USER     %5d |\n",si->si_code);
				break;
		case SI_KERNEL:		printf("|           | si_code   |    SI_KERNEL   %5d |\n",si->si_code);
				break;
		case SI_QUEUE:		printf("|           | si_code   |    SI_QUEUE    %5d |\n",si->si_code);
				break;
		case SI_TIMER:		printf("|           | si_code   |    SI_TIMER    %5d |\n",si->si_code);
				break;
		case SI_MESGQ:		printf("|           | si_code   |    SI_MESGQ    %5d |\n",si->si_code);
				break;
		case SI_ASYNCIO:	printf("|           | si_code   |    SI_ASYNCIO  %5d |\n",si->si_code);
				break;
		case SI_SIGIO:		printf("|           | si_code   |    SI_SIGIO    %5d |\n",si->si_code);
				break;
		default: 			printf("|           | si_code   |    SI_ ???     %5d |\n",si->si_code);
	}
	printf("|           | si_pid    | %20d |\n",si->si_pid);
	printf("|           | si_uid    | %20d |\n",si->si_uid);
	printf("|           | si_status | %20d |\n",si->si_status);
	printf("|           | si_utime  | %5ld:%03ld %10d |\n",
			si->si_utime/CLOCKS_PER_SEC, si->si_utime*1000/CLOCKS_PER_SEC%1000, (int)si->si_utime);
	printf("| siginfo_t | si_stime  | %5ld:%03ld %10d |\n",
			si->si_stime/CLOCKS_PER_SEC, si->si_stime*1000/CLOCKS_PER_SEC%1000, (int)si->si_stime);
	printf("|           | si_value  |    int:%-4d ptr:%-4d |\n",si->si_value.sival_int,(int)si->si_value.sival_ptr);
	printf("|           | si_int    | %20d |\n",si->si_int);
	printf("|           | si_ptr    | %20d |\n",(int)si->si_ptr);
	printf("|           | si_addr   | %20d |\n",(int)si->si_addr);
	printf("|           | si_band   | %20ld |\n",si->si_band);
	printf("|           | si_fd     | %20d |\n",si->si_fd);
	printf("+-----------+-----------+----------------------+\n\n");
}
