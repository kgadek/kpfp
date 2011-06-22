#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include "heap.h"


#ifndef KLIENTCZEKANADOSTAWCOW
#define KLIENTCZEKANADOSTAWCOW 1
#endif
#define MAXILOSCTOWAROW 1000
#define KLIENTSLEEP (rand()%3)
#define DOSTAWCASLEEP (rand()%5)
#define myerror(str,myerrno) {\
                        fprintf(stderr,"Err #%03d (errno=%d):\n\t%s\n",myerrno,errno,str);\
                        exit(myerrno);\
                }
#define NEW(x) ((x*)malloc(sizeof(x)))
#define NEWTAB(x,N) ((x*)malloc(((unsigned int)N)*sizeof(x)))
#define GETM(mutex) pthread_mutex_lock(&mutex)
#define RELM(mutex) pthread_mutex_unlock(&mutex)


typedef unsigned int uint;


void* klient(void*); /* wchodzi i kupuje */
void* dostawca(void*); /* jest wstydliwy i nie wejdzie do sklepu gdy są w nim klienci; dostarcza sklepowi towarów ale też pobiera należności */


int klientCntr; /*ilość klientów w sklepie*/
pthread_cond_t czysto; /*sygnał, że w sklepie jest czysto*/
pthread_mutex_t klamka; /*tylko jedna osoba może trzymać klamkę (czyli atomowe wchodzenie/wychodzenie ze sklepu) */
pthread_mutex_t parking; /*parking dla dostawców*/
struct kpfp_heap towary;


int main(int argc, char **argv) {
	int klThC;
	int doThC;
	int i;
	pthread_t *klTh;
	pthread_t *doTh;

	/*init*/
	pthread_mutex_init(&klamka, NULL);
	pthread_mutex_init(&parking, NULL);
	pthread_cond_init(&czysto, NULL);
	klientCntr = 0;
	srand((unsigned int)time(NULL));
	kpfp_heap_init(&towary, MAXILOSCTOWAROW);

	/*parse*/
	if(argc != 3)
		myerror("Podano nieprawidłową ilość argumentów (podaj ilość klientów i dostawców).",1);
	klThC = atoi(argv[1]);
	doThC = atoi(argv[2]);

	/*go!*/
	klTh = NEWTAB(pthread_t, klThC);
	doTh = NEWTAB(pthread_t, doThC);
	for(i=0; i<klThC; ++i)
		pthread_create(&klTh[i], NULL, klient, NULL);
	for(i=0; i<doThC; ++i)
		pthread_create(&doTh[i], NULL, dostawca, NULL);

	/*wait (forever) and clean...*/
	for(i=0; i<klThC; ++i)
		pthread_join(klTh[i], NULL);
	for(i=0; i<doThC; ++i)
			pthread_join(doTh[i], NULL);
	free(klTh);
	free(doTh);

	return 0;
}

void* klient(void *arg) {
#if KLIENTCZEKANADOSTAWCOW
	GETM(parking);
	RELM(parking);
#endif
	sleep((uint)KLIENTSLEEP);
	GETM(klamka);
		printf("Klient %d wchodzi do sklepu\n", (uint)pthread_self());
		++klientCntr;
	RELM(klamka);

	/*klientowanie*/
	printf("Klient %d znalazł najdroższy towar za %d zł.\n", (uint)pthread_self(), kpfp_heap_getMax(&towary));

	GETM(klamka);
		if(!--klientCntr) {
			printf("Klient %d wychodząc informuje: sklep pusty!\n",(uint)pthread_self());
			pthread_cond_signal(&czysto); /*jeśli wychodząc okaże się, że klient był ostatni to mówi o tym ew. dostawcy (wszyscy w mieście wiedzą, że dostawcy są wstydliwi)*/
		} else
			printf("Klient %d wychodzi ze sklepu\n", (uint)pthread_self());
	RELM(klamka);
	return NULL;
}

void* dostawca(void *arg) {
	int i;
	printf("Dostawca %d na horyzoncie!\n", (uint)pthread_self());
	GETM(parking); /*blokuj -- dostawcy szukają miejsc parkingowych (jest jedno!), tylko pierwszy zaparkował i może sprawdzać czy jest pusto w sklepie*/
	printf("Dostawca %d zaparkował i czeka przed sklepem\n", (uint)pthread_self());
	GETM(klamka);
		while(klientCntr)
			pthread_cond_wait(&czysto, &klamka);

		/*dostawa*/
		printf("Dostawa towaru: ");
		if(rand()%3) {
			i = rand()%10000;
			kpfp_heap_push(&towary,i);
			printf("nowy towar za %d złotych", i);
		} else
			printf("zabrano nierentowny towar za %d złotych", kpfp_heap_popMax(&towary));
		sleep((uint)DOSTAWCASLEEP);
		printf("... wykonano\n");

	RELM(klamka);
	RELM(parking);
	return NULL;
}
