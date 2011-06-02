#define _GNU_SOURCE
#include "common.h"

void myatexit(void);
char getRandomChar(void);

int myQueue;

int main(int argc, char **argv) {
	key_t servKey;
	int tmp;
	int i;
	int j;
	int servQueue;
	int msgCnt = 0;
	const char *myName;
	clMsg myMsg;
	svMsg hisMsg;

	srand((uint)time(0));
	if(argc != 2)
		myerror("Nieprawidłowa ilość argumentów! (Podaj jeden -- nazwę klienta)",1);
	myName = argv[1];

	servKey = ftok("serwer",71);
	if(servKey == -1)
		myerror("Błąd ftok!",2);
	servQueue = msgget(servKey,0);
	if(servQueue == -1)
		myerror("Błąd msgget!",3);
	myQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0755);
	if(myQueue == -1)
		myerror("Błąd msgget!",4);
	tmp = atexit(myatexit);
	if(tmp != 0) {
		myatexit();
		myerror("Błąd atexit!",5);
	}

	for(;;) { /*klientujemy*/
		printf("Ile mesydżów wysłać? : ");
		scanf(" %d",&msgCnt);
		if(msgCnt < 0) {
			printf("\nBłąd -- podano ujemną liczbę!\n");
			continue;
		}
		for(i=0;i<msgCnt;++i) {
			myMsg.type = rand() % 20;
			myMsg.myQueueNum = myQueue;
			strncpy(myMsg.myNameIs,myName,CLNAMELEN);
			myMsg.myNameIs[CLNAMELEN-1] = 0;
			j = rand()%CLMSGLEN;
			myMsg.iWantToSay[j+1] = 0;
			for(; j >= 0; --j)
				myMsg.iWantToSay[j] = getRandomChar();
			showClMsg(&myMsg,0);
			tmp = msgsnd(servQueue,&myMsg,sizeof(myMsg),0);
			if(tmp == -1)
				myerror("Błąd msgsnd!",6);
			tmp = msgrcv(myQueue,&hisMsg,sizeof(svMsg),0,0);
			if(tmp == -1)
				myerror("Błąd msgrcv!",7);
			showSrvMsg(&hisMsg, "Odpowiedź serwera:");
			hr();
		}
	}

	return 0;
}

void myatexit(void) {
	int res = msgctl(myQueue,IPC_RMID,0);
	if(res == -1)
		printf("Błąd wyjścia -- nie mogę usunąć kolejki #%03d -- errno=%d\n",myQueue,errno);
}

char getRandomChar() {
	char res = (char)(rand()%('z'-6-'A')+'A');
	if(res > 'Z')
		res = (char)(res + 6); /* pomijam znaki z poza a-zA-Z */
	return res;
}


