#define _GNU_SOURCE
#include "common.h"

void myatexit(void);
void sigSigSigSigSigSig(int);

uint LS = 0;
uint msgCnt = 0;
mqd_t KS_h = -1;
mqd_t KK_h = -1;

int main() {
	int tmp;
	struct sigevent njuMesydz;
	struct mq_attr queueAttrib;

	srand((uint)time(0));

	tmp = (int)signal(SIGINT, makeMeQuit);
	if(tmp == (int)SIG_ERR)
		myerror("Błąd signal (SIGINT)!",1);
	tmp = atexit(myatexit);
	if(tmp != 0) {
		myatexit();
		myerror("Błąd atexit!",5);
	}

	queueAttrib.mq_maxmsg = 20;
	queueAttrib.mq_msgsize = sizeof(clMsg);
	KS_h = mq_open(KSNAME,O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK, 0755, &queueAttrib);
	if(KS_h == (mqd_t)-1)
		myerror("Błąd mq_open (KS)!",1);
	queueAttrib.mq_msgsize = sizeof(svMsg);
	KK_h = mq_open(KKNAME,O_WRONLY | O_CREAT | O_EXCL | O_NONBLOCK, 0755, &queueAttrib);
	if(KK_h == (mqd_t)-1) {
		tmp = mq_close(KS_h);
		if(tmp == -1)
			fprintf(stderr,"Błąd zamknięcia kolejki KS! errno=%d\n",errno);
		myerror("Błąd mq_open (KK)!",3);
	}

	njuMesydz.sigev_notify = SIGEV_SIGNAL;
	njuMesydz.sigev_signo = SIGUSR1;
	tmp = (int)signal(SIGUSR1, sigSigSigSigSigSig);
	if(tmp == (int)SIG_ERR)
		myerror("Błąd signal (SIGUSR1)!",4);
	tmp = mq_notify(KS_h, &njuMesydz);
	if(tmp == -1)
		myerror("Błąd mq_notify()!",2);

	for(;;) { /* serwerujemy */
		sleep(1);
		++LS;
		printf("LS = %d msgCnt=%d\n",LS,msgCnt);
	}

	return 0;
}

void myatexit(void) {
	int tmp;
	tmp = mq_close(KK_h);
	if(tmp == -1)
		fprintf(stderr, "Błąd mq_close(KK_h)!... errno=%d\n",errno);
	tmp = mq_unlink(KKNAME);
	if(tmp == -1)
		fprintf(stderr, "Błąd mq_unlink(KKNAME)!... errno=%d\n",errno);
	tmp = mq_close(KS_h);
	if(tmp == -1)
		fprintf(stderr, "Błąd mq_close(KS_h)!... errno=%d\n",errno);
	tmp = mq_unlink(KSNAME);
	if(tmp == -1)
		fprintf(stderr, "Błąd mq_unlink(KSNAME)!... errno=%d\n",errno);

}

void sigSigSigSigSigSig(int sigId) {
	int tmp;
	int size;
	clMsg inc;
	svMsg out;
	struct sigevent njuMesydz;

	while(1){
		size = mq_receive(KS_h, (char*)&inc, sizeof(clMsg), 0);
		if(size == -1) {
			if(errno == EAGAIN) { /*pusta kolejka*/
				break;
			}
			myerror("Błąd mq_receive!",7);
		}
		showClMsg(&inc,0);

		out.type = 666;
		out.LS = LS;
		out.msgSent = msgCnt;
		strncpy(out.hisNameIs, inc.myNameIs, CLNAMELEN);
		strncpy(out.heSaid, inc.iWantToSay, CLMSGLEN);
		showSrvMsg(&out,"A ja na to jak na lato:");

		tmp = mq_send(KK_h, (char*)&out, sizeof(svMsg),0);
		if(tmp == -1)
			myerror("Błąd mq_send!",10);
		++msgCnt;
	}

	njuMesydz.sigev_notify = SIGEV_SIGNAL;
	njuMesydz.sigev_signo = SIGUSR1;
	tmp = mq_notify(KS_h, &njuMesydz);
	if(tmp == -1)
		myerror("Błąd mq_notify()!",8)
	tmp = (int)signal(SIGUSR1, sigSigSigSigSigSig);
	if(tmp == (int)SIG_ERR)
		myerror("Błąd signal (SIGUSR1)!",9);

	kill(inc.myCard, SIGUSR1);
}
