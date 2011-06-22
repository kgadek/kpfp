#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include <pthread.h>

#define KLIENTCZEKANADOSTAWCOW 0

#define GETM(mutex) pthread_mutex_lock(&mutex)
#define RELM(mutex) pthread_mutex_unlock(&mutex)

void* klient(void*); /* wchodzi i kupuje */
void* dostawca(void*); /* jest wstydliwy i nie wejdzie do sklepu gdy są w nim klienci; dostarcza sklepowi towarów ale też pobiera należności */


int klientCntr; /*ilość klientów w sklepie*/
pthread_cond_t czysto; /*sygnał, że w sklepie jest czysto*/
pthread_mutex_t klamka; /*tylko jedna osoba może trzymać klamkę (czyli atomowe wchodzenie/wychodzenie ze sklepu) */
pthread_mutex_t parking; /*parking dla dostawców*/

int main(int argc, char **argv) {
	/*init*/
	pthread_mutex_init(&klamka, NULL);
	pthread_mutex_init(&parking, NULL);
	pthread_cond_init(czysto, NULL);
	klientCntr = 0;
	return 0;
}

void* klient(void *arg) {
#if KLIENTCZEKANADOSTAWCOW
	GETM(parking);
	RELM(parking);
#endif
	GETM(klamka);
		++klientCntr;
	RELM(klamka);
	// klientowanie
	GETM(klamka);
		if(!--klientCntr)
			pthread_cond_signal(&czysto); /*jeśli wychodząc okaże się, że klient był ostatni to mówi o tym ew. dostawcy (wszyscy w mieście wiedzą, że dostawcy są wstydliwi)*/
	RELM(klamka);
}

void* dostawca(void *arg) {
	GETM(parking); /*blokuj -- dostawcy szukają miejsc parkingowych (jest jedno!), tylko pierwszy zaparkował i może sprawdzać czy jest pusto w sklepie*/
	GETM(klamka);
		while(!klientCntr)
			pthread_cond_wait(&czysto, &klamka);
		// dostawa
	RELM(klamka);
	RELM(parking);
}
