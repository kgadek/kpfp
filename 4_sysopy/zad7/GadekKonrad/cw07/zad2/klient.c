#define _GNU_SOURCE
#include "common.h"

void myatexit(void);
void sigSigSigSigSigSig(int);

mqd_t KS_h = -1;
mqd_t KK_h = -1;

int main(int argc, char **argv) {
	int tmp;
	int i;
	int j;
	int msgCnt = 0;
	clMsg myMsg;

	srand((uint)time(0));
	if(argc != 2)
		myerror("Nieprawidłowa ilość argumentów! (Podaj jeden -- nazwę klienta)",1);

	myMsg.type = 72;
	strncpy(myMsg.myNameIs, argv[1], CLNAMELEN);
	myMsg.myNameIs[CLNAMELEN-1] = 0;
	myMsg.myCard = getpid();

	signal(SIGINT, makeMeQuit); /*och proszę, niech będą te dwa polecenia atomowe...*/
	tmp = atexit(myatexit);
	if(tmp != 0) {
		myatexit();
		myerror("Błąd atexit()!",2);
	}

	KS_h = mq_open(KSNAME,O_WRONLY);
	if(KS_h == (mqd_t)-1)
		myerror("Błąd mq_open (KS)!",1);
	KK_h = mq_open(KKNAME,O_RDONLY);
	if(KK_h == (mqd_t)-1) {
		tmp = mq_close(KS_h);
		if(tmp == -1)
			fprintf(stderr,"Błąd zamknięcia kolejki KS! errno=%d\n",errno);
		myerror("Błąd mq_open (KK)!",3);
	}

	signal(SIGUSR1, sigSigSigSigSigSig);
	for(;;) { /*klientujemy*/
		printf("Ile mesydżów wysłać? : ");
		scanf(" %d",&msgCnt);
		if(msgCnt < 0) {
			printf("\nBłąd -- podano ujemną liczbę!\n");
			continue;
		}

		for(i=0;i<msgCnt;++i) {
			j = rand()%CLMSGLEN;
			myMsg.iWantToSay[j+1] = 0;
			for(; j >= 0; --j)
				myMsg.iWantToSay[j] = getRandomChar();
			showClMsg(&myMsg,0);
			tmp = mq_send(KS_h, (char*)&myMsg, sizeof(clMsg), 0);
			if(tmp == -1)
				myerror("Błąd mq_send!",4);
			hr();
		}
	}

	return 0;
}

void myatexit(void) {
	int tmp = 0;
	if(KK_h != -1)
		tmp = mq_close(KK_h);
	if(tmp == -1)
		fprintf(stderr,"Błąd zamknięcia kolejki KK! errno=%d\n",errno);
	tmp = 0;
	if(KS_h != -1)
		tmp = mq_close(KS_h);
	if(tmp == -1)
		fprintf(stderr,"Błąd zamknięcia kolejki KS! errno=%d\n",errno);
}

void sigSigSigSigSigSig(int sigId) {
	svMsg hisMsg;
	int size;

	hr();
	size = mq_receive(KK_h, (char*)&hisMsg, sizeof(svMsg),0);
	if(size == -1)
		myerror("Błąd w obsłudze sygnału (sigSigSigSigSigSig)", 5);
	showSrvMsg(&hisMsg, "Wiadomość! Wiadomość! Mam wiadomość! Dostałem! TAK! Wiadomość!");
	hr();

	signal(SIGUSR1, sigSigSigSigSigSig);
}
