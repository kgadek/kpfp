/*Zadanie 3

a) Napisz dwa programy - program wysyłający sygnały SIGUSR1 (przodek) i program zliczający ilość
odebranych sygnałów (potomek). Ilość sygnałów SIGUSR1 wysyłanych przez pierwszy program powinna być
określana w parametrze wywołania tego programu. Po transmisji sygnałów SIGUSR1 proces pierwszy powinien
wysłać sygnał SIGUSR2, który powinien zakończyć wykonanie procesu drugiego, poprzedzone wypisaniem
komunikatu na temat ilości odebranych sygnałów SIGUSR1. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler1(int,siginfo_t*,void*);
void handler2(int);

int cnt;

int main() {
	struct sigaction aH1;
	struct sigaction aH2;

	cnt = 0;

	aH1.sa_sigaction = handler1;
	aH1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &aH1, NULL);
	aH2.sa_handler = handler2;
	sigaction(SIGUSR2, &aH2, NULL);

	while(1)
		pause();

	return 0;
}

void handler1(int sigid,siginfo_t *si,void *cont) {
	union sigval sv;
	sv.sival_int = ++cnt;
	sigqueue(si->si_pid, sigid, sv);
}

void handler2(int sigid) {
	printf("SIGUSR1 cnt=%d\n",cnt);
	cnt = 0;
	exit(0);
}
