/*Zadanie 3

a) Napisz dwa programy - program wysyłający sygnały SIGUSR1 (przodek) i program zliczający ilość
odebranych sygnałów (potomek). Ilość sygnałów SIGUSR1 wysyłanych przez pierwszy program powinna być
określana w parametrze wywołania tego programu. Po transmisji sygnałów SIGUSR1 proces pierwszy powinien
wysłać sygnał SIGUSR2, który powinien zakończyć wykonanie procesu drugiego, poprzedzone wypisaniem
komunikatu na temat ilości odebranych sygnałów SIGUSR1.

c) Zmodyfikuj programy opisane w punkcie a) tak, by przesyłany sygnał był sygnałem czasu rzeczywistego
(patrz man -s 7 signal). Jaka liczba sygnałów będzie teraz odebrana? */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler1(int,siginfo_t*,void*);
void handler2(int,siginfo_t*,void*);

unsigned int cnt;

int main() {
	struct sigaction aH1;
	struct sigaction aH2;

	cnt = 0;

	aH1.sa_flags = SA_SIGINFO;
	aH1.sa_sigaction = handler1;
	sigaction(SIGRTMIN+2, &aH1, NULL);
	aH2.sa_flags = SA_SIGINFO;
	aH2.sa_sigaction = handler2;
	sigaction(SIGRTMIN+1, &aH2, NULL);

	while(1)
		pause();

	return 0;
}

void handler1(int sigid,siginfo_t *si,void *sc) {
	++cnt;
}

void handler2(int sigid,siginfo_t *si,void *sc) {
	printf("SIGRTMIN+2 cnt=%d\n",cnt);
	exit(0);
}
