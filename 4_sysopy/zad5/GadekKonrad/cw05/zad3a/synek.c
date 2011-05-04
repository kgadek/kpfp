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
#include <unistd.h>

void handler1(int);
void handler2(int);

unsigned int cnt;

int main() {
	struct sigaction aH1;
	struct sigaction aH2;

	cnt = 0;

	aH1.sa_handler = handler1;
	sigaction(SIGUSR1, &aH1, NULL);
	aH2.sa_handler = handler2;
	sigaction(SIGUSR2, &aH2, NULL);

	while(1)
		pause();

	return 0;
}

void handler1(int sigid) {
	++cnt; /*po cichu udajemy, że to jest atomowe...*/
}

void handler2(int sigid) {
	printf("SIGUSR1 cnt=%d\n",cnt);
	exit(0);
}
