/*Zadanie 3

a) Napisz dwa programy - program wysyłający sygnały SIGUSR1 (przodek) i program zliczający ilość
odebranych sygnałów (potomek). Ilość sygnałów SIGUSR1 wysyłanych przez pierwszy program powinna być
określana w parametrze wywołania tego programu. Po transmisji sygnałów SIGUSR1 proces pierwszy powinien
wysłać sygnał SIGUSR2, który powinien zakończyć wykonanie procesu drugiego, poprzedzone wypisaniem
komunikatu na temat ilości odebranych sygnałów SIGUSR1.

b) Do programów w zestawie a) dodaj potwierdzenie odbioru sygnału. W tym celu, niech proces drugi
wysyła do procesu pierwszego sygnał SIGUSR1 informujący o odbiorze sygnału. Proces pierwszy powinien
wysłać kolejny sygnał dopiero po uzyskaniu tego potwierdzenia. Zapewnij rozwiązanie, w którym ilość
sygnałów odebranych jest zgodna z ilością sygnałów wysłanych i w którym nie dochodzi do zakleszczenia. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

typedef unsigned int uint;
int waiter; 					/*stan kelnera czekającego na potw. zamówienia*/

void ack(int);

int main(int argc, char **argv) {
	pid_t pid;					/*pid dzieciaka*/
	uint n;						/*ile sygnałów do wysłania*/
	char *p;					/*do wykrywania blędów konwersji string->liczba*/
	union sigval sv;			/*dodatkowe informacje przesyłane wraz z sigqueue*/
	struct sigaction sigAck;	/*do rejestracji potwierdzacza*/
	sigset_t maskUsr1;			/*maskowanie SIGUSR1*/
	sigset_t maskOld;			/*domyślna maska*/

	sigemptyset(&maskUsr1);
	sigaddset(&maskUsr1, SIGUSR1);

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

	sigfillset(&sigAck.sa_mask);						/*rejestracja potwierdzacza zamówień*/
	sigAck.sa_handler = ack;
	sigaction(SIGUSR1, &sigAck, NULL);

	sv.sival_int = 0;					/*,,A wiecie że dali mi... zero napiwku? Skąpce!''*/
	sigprocmask(SIG_BLOCK, &maskUsr1, &maskOld);		/*blokujemy SIGUSR1*/
	while(n--) {
		waiter = 1;
		sigqueue(pid,SIGUSR1,sv); 		/*,,E! Czekolada na gorąco dla stolika 4.''*/
														/*ISO C forbids casting to union type*/
														/*więc (union sigval)0 nie przechodzi*/
		while(waiter) 					/*,,...to może poczekam aż zrobicie tą czekoladę''*/
			sigsuspend(&maskOld);						/*SIGUSR1 odblokowane tylko wewnątrz*/
	}
	sigprocmask(SIG_UNBLOCK, &maskUsr1, NULL);			/*odblokowujemy SIGUSR1*/
	sigqueue(pid,SIGUSR2,sv);			/*hasło ,,Przerwa!'' nie wymaga potwierdzenia*/

	return 0;
}

void ack(int sigid) {
	waiter = 0;							/*,,Ok, to teraz następny klient. Może on da napiwek?''*/
}
