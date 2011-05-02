/*Zadanie 2

Napisz dwa programy: sender i receiver. Niech sender wysyła sygnał SIGUSR1 przy użyciu funkcji kill
lub sigqueue. Wybór jednej z tych funkcji powinien być dokonywany na podstawie parametru przekazanego
w argumencie wywołania programu. W przypadku zastosowania funkcji sigqueue w argumentach programu musi
wystąpić jeszcze jeden parametr, zawierający wartość (liczbę całkowitą), która będzie przesłana wraz z
sygnałem. Program receiver powinien odbierać przesłany sygnał, wypasując na ekranie odpowiedni
komunikat wraz z dodatkowymi informacjami o sygnale (rzeczywisty identyfikator użytkownika
uruchamiającego proces sender, PID procesu sender oraz przekazany czas użytkownika). W przypadku
przesyłania z sygnałem dodatkowej wartości, receiver również wypisuje ją na ekranie. */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
	return 0;
}
