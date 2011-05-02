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
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

void send(int,pid_t,int);

int main(int argc, char **argv) {
	int fun; /*kill czy sigqueue czy nierozpoznane -- 1 lub 2 lub 0*/
	int val; /*jeśli sigqueue -- wartość do przesłania (podano gddy fun==2 && argc > 2)*/
	pid_t pid; /*pid o ile podano (gddy argc == fun+2)*/
	FILE *pp;

	fun = 0; /*stwierdź którą funkcję wywołać*/
	if(argc > 1) {
		if(!strcmp(argv[1],"kill"))
			fun = 1;
		else if(!strcmp(argv[1],"sigqueue"))
			fun = 2;
	}
	if(!fun || argc > 4 || (fun==1 && argc == 4)) { /*o dziwo to pokrywa chyba wszystkie przypadki błędów*/
		fprintf(stderr,	"Błędne wywołanie programu!\n"\
						"Użycie:\n"\
							"\t%s kill [PID] -- wyślij sygnał SIGUSR1 do receivera\n"\
								"\t\t\to podanym PID przy pomocy kill(2) lub do\n"
								"\t\t\twszystkich uruchomionych receiverów (gdy nie podano PID)\n"\
							"\t%s sigqueue [val] [pid] -- podobnie j/w; różnice:\n"\
								"\t\t\t - użycie sigqueue(2) zamiast kill(2)\n"\
								"\t\t\t - można podać wartość, która zostanie przesłana\n"\
								"\t\t\t   do receivera\n",
							argv[0], argv[0]);
		exit(1);
	}
	val = 0; /*nie sprawdzam już poprawności poniższych danych*/
	if(fun==2 && argc > 2)
		val = atoi(argv[2]);
	pid = 0;
	if(argc == fun + 2)
		pid = atoi(argv[fun+1]);

	if(pid) { /*jeśli podano PID*/
		if(kill(pid,0) == -1 && errno == ESRCH) {
			fprintf(stderr,"Podano nieprawidłowy PID: %d\n",pid);
			exit(errno);
		}
		send(fun,pid,val);
	} else { /*jeśli wysyłamy do wszystkich receiverów*/
		if(!(pp = popen("pidof receiver","r"))) {
			fprintf(stderr,"Nie można wywołać: pidof receiver\nerr: %d\n",errno);
			exit(errno);
		}
		pid = 0;
		while(fscanf(pp, " %d ", &pid) != EOF)
			send(fun,pid,val);
		pclose(pp);
		if(!pid) /*jeśli pid jest nieruszony to fscanf(...) nic nie wczytał*/
			printf("Nie wysłano nigdzie!\n");
	}


	return 0;
}

void send(int f, pid_t p,int v) {
	union sigval vu;
	if(f==1)
		kill(p,SIGUSR1);
	else {
		vu.sival_int = v;
		sigqueue(p,SIGUSR1,vu);
	}

}
