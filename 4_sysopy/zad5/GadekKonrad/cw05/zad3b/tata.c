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

typedef unsigned int uint;
int waiter; /*stan kelnera czekającego na potw. zamówienia*/

void ack(int);

int main(int argc, char **argv) {
	pid_t pid;
	uint n;
	char *p; /*do wykrywania blędów konwersji*/
	union sigval sv;
	struct sigaction sigAck;

	printf("start\n");
	if(argc == 3) {
		pid = (pid_t)strtol(argv[1], &p, 10);
		if(p!=argv[1])
			n = (uint)strtol(argv[2], &p, 10);
	}
	if(argc != 3 || p == argv[1] || p == argv[2]) {
		fprintf(stderr,"Błędne wywołanie programu!\n"\
						"Użycie:\n"\
							"\t%s PID N\t-- wyślij do syna o określonym PID sygnał SIGUSR1 N-razy\n"\
									"\t\t\ta potem SIGUSR2 jeden raz\n",
						argv[0]);
		exit(1);
	}

	printf(">>>");
	sigfillset(&sigAck.sa_mask);
	sigAck.sa_handler = ack; /*rejestracja potwierdzacza zamówień*/
	sigaction(SIGUSR1, &sigAck, NULL);

	printf("pre-while\n");
	sv.sival_int = 0;
	while(n--) {
		printf(".");
		waiter = 1;
		sigqueue(pid,SIGUSR1,sv); /*,,E! Czekolada na gorąco dla stolika 4.''*/
					/*ISO C forbids casting to union type*/
					/*więc (union sigval)0 nie przechodzi*/
		while(waiter);		/*CPU-burner czyli ,,poczekam aż zrobicie tą czekoladę''*/
	}
	sigqueue(pid,SIGUSR2,sv);	/*hasło ,,Przerwa!'' nie wymaga potwierdzenia*/
	/*kill(pid,SIGUSR2);*/

	return 0;
}

void ack(int sigid) {
	printf(",");
	waiter = 0;
}
