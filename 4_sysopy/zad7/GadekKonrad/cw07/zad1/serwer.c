#define _GNU_SOURCE
#include "common.h"

void myatexit(void);

int myQueue;
enum queueType {UNKNOWN = 1, FIFO = 0, PRIORITY = -1000};

int main(int argc, char **argv) {
	int maxSleepTime;
	enum queueType myQueueType = UNKNOWN;
	key_t servKey;
	int tmp;
	clMsg inc;
	svMsg out;
	struct msqid_ds queueStats;

	srand((uint)time(0));
	if(argc != 3)
		myerror("Nieprawidłowa ilość argumentów! ([f/p] maxSleepTime)",1);

	maxSleepTime = atoi(argv[2]);
	if(maxSleepTime < 0)
		myerror("Podano ujemną wartość oczekiwania!",2);
	++maxSleepTime;
	if(!strcmp(argv[1],"p"))
		myQueueType = PRIORITY;
	else if(!strcmp(argv[1],"f"))
		myQueueType = FIFO;
	if(myQueueType == UNKNOWN)
		myerror("Podano nieprawidłową nazwę typu kolejki [f/p] - fifo/priority!",3);

	servKey = ftok("serwer",71);
	if(servKey == -1)
		myerror("Błąd ftok!",4);
	myQueue = msgget(servKey,IPC_CREAT | 0755);
	if(servKey == -1)
		myerror("Błąd msgget!",5);
	tmp = atexit(myatexit);
	if(tmp != 0) {
		myatexit();
		myerror("Błąd atexit!",5);
	}

	for(;;) { /* serwerujemy */
		tmp = msgrcv(myQueue, &inc, sizeof(clMsg), myQueueType, 0);
		if(tmp == (ssize_t)-1)
			myerror("Błąd msgrcv!",6);
		showClMsg(&inc,"Otrzymałem od klienta nast. wiadomość");
		printf("Zbieram statystyki...\n");
		tmp = msgctl(myQueue, IPC_STAT, &queueStats);
		if(tmp == -1)
			myerror("Błąd msgctl (IPC_STAT)!",7);
		tmp = rand() % maxSleepTime;
		printf("Usypiam na %d sek... Dobranoc!\n",tmp);
		sleep((uint)tmp);
		out.type = 1;
		strncpy(out.hisNameIs, inc.myNameIs, CLNAMELEN);
		strncpy(out.heSaid, inc.iWantToSay, CLMSGLEN);
		out.msgInQueue = queueStats.msg_qnum;
		out.bytesInQueue = queueStats.__msg_cbytes;
		out.maxBytesInQueue = queueStats.msg_qbytes;
		out.pidReader = queueStats.msg_lrpid;
		out.pidSender = queueStats.msg_lspid;
		out.lastRead = queueStats.msg_rtime;
		out.lastSend = queueStats.msg_stime;
		showSrvMsg(&out,0);
		tmp = msgsnd(inc.myQueueNum, &out, sizeof(svMsg),0);
		if(tmp == -1)
			myerror("Błąd msgsnd!",8);
		hr();
	}

	return 0;
}

void myatexit(void) {
	int res = msgctl(myQueue,IPC_RMID,0);
	if(res == -1)
		printf("Błąd wyjścia -- nie mogę usunąć kolejki #%03d -- errno=%d\n",myQueue,errno);
}
