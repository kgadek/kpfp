#include "common.h"
#define MYPAFMAXLEN 100


void kaczIo(int);


struct kpfpMsg msg;
int sockFd;
size_t addrSi;
struct sockaddr *addrSo;
char name[MAXNAMELEN];


int main(int argc, char **argv) {
	char clPaf[MYPAFMAXLEN];
	struct sockaddr_un clAddr;
	struct sockaddr_un svAddr;
	struct sockaddr_in clNetAddr;
	struct sockaddr_in svNetAddr;
	int tmp;
	int myPort;
	pid_t pid;
	int goToSleep;

	/*init*/
	srand((uint)time(0));
	pid = getpid();
	snprintf(clPaf, MYPAFMAXLEN, "/tmp/kpfp_csock%d",pid);
	printf("Mój sockecik: %s\n", clPaf);
	myPort = rand()%256 + 2000;

	/*kacz argz*/
	if(argc < 2)
		myerror("Błędne wywołanie. Oczekuję:\n\tself [net _ip _port | local _path]",1);
	if(!strcmp(argv[1], "local")) {
		if(argc != 3)
			myerror("Błędna ilość argumentów.", 4);
		sockFd = socket(PF_UNIX, SOCK_DGRAM, 0);
		if(sockFd == -1)
			myerror("Błąd socket()",3);
		svAddr.sun_family = clAddr.sun_family = AF_UNIX;
		snprintf(clAddr.sun_path, 108-1, clPaf);
		snprintf(svAddr.sun_path, 108-1, argv[2]);
		tmp = bind(sockFd, (struct sockaddr*)&clAddr, SUN_LEN(&clAddr));
		if(tmp == -1)
			myerror("Błąd bind",5);
		addrSi = SUN_LEN(&svAddr);
		addrSo = (struct sockaddr*)&svAddr;
		snprintf(name, MAXNAMELEN, "%u.%s", pid, "local");
	} else if (!strcmp(argv[1],"net")) {
		if(argc != 4)
			myerror("Błędna ilość argumentów.", 7);
		sockFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(sockFd == -1) {
			myerror("Błąd socket!",6);
		} else
			printf("Socket ok\n");
		svNetAddr.sin_family = clNetAddr.sin_family = AF_INET;
		clNetAddr.sin_port = htons((uint16_t)myPort);
		clNetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		tmp = bind(sockFd, (struct sockaddr*)&clNetAddr, sizeof(clNetAddr));
		if(tmp == -1) {
			myerror("Błąd bind",123);
		} else
			printf("Bind ok\n");
		tmp = 1;
		tmp = setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
		if(tmp == -1) {
			myerror("Błąd setsockopt",8);
		} else
			printf("setsockopt OK\n");
		svNetAddr.sin_port = htons((uint16_t)atoi(argv[3]));
		tmp = inet_aton(argv[2], &(svNetAddr.sin_addr));
		if(tmp == -1)
			myerror("Błąd inet_aton",9);
		addrSi = sizeof(svNetAddr);
		addrSo = (struct sockaddr*)&svNetAddr;
		snprintf(name, MAXNAMELEN, "%u.%s", pid, "internet");
	} else {
		myerror("Błędny pierwszy argument!",2);
	}

	strncpy(msg.command, "registerMe", (uint)MAXCOMMANDSIZE);
	snprintf(msg.buf, MAXBUFSIZE, "%u", addrSi);
	strncpy(msg.name,name, MAXNAMELEN);
	printf("AAA\n");
	tmp = sendto(sockFd, &msg, sizeof(msg), 0, addrSo, addrSi);
	if(tmp == -1)
		myerror("Błąd sendto",10);
	printf("BBB\n");
	tmp = recvfrom(sockFd, &msg, sizeof(msg), 0, addrSo, &addrSi);
	printf("CCC\n");
	printf("Odpowiedź: (%s:%s) %s\n", msg.name, msg.command, msg.buf);

	tmp = 1;
	tmp = ioctl(sockFd, FIOASYNC, &tmp);
	if(tmp == -1)
		myerror("Błąd ioctl -- FIOASYNC", 11);
	tmp = ioctl(sockFd, SIOCSPGRP, &pid); /*sanity check*/
	if(tmp == -1)
		myerror("Błąd ioctl -- SIOCSPGRP", 12);
	signal(SIGIO, kaczIo);

	printf("Witamy w ubogim REPL! Jak coś zawiesisz to będzie Twoja wina!\n");
	goToSleep = 0;
	while(!goToSleep) {
		printf("Komendy:\n\t0 - logałt und kłit\n\t1 - pytaj czy jest ktokolwiek kto mógłby mieć problem\n\t2 - zapytaj czy konkretny klient ma problem\n");
		scanf(" %d",&tmp);
		switch(tmp) {
		case 0: /*gary muwałt stąd*/
			printf("No to papatki...\n");
			strncpy(msg.command, "logout", (uint)MAXCOMMANDSIZE);
			goToSleep = 1;
			break;
		case 1: /*czy ktokolwiek ma problem?*/
			printf("Ey, k***a! Macie problem?\n");
			strncpy(msg.command, "list", (uint)MAXCOMMANDSIZE);
			break;
		case 2:
			printf("Ey, Ty! Tak, do ciebie k***a mówię! Masz problem?\n");
			strncpy(msg.command, "ask", (uint)MAXCOMMANDSIZE);
			printf("\t(Podaj, kto ma mieć problem)\n");
			scanf(" %s", msg.buf);
			break;
		default:
			printf("Nieprawidłowe polecenie!\n");
			continue;
		}
		strncpy(msg.name, name, (uint)MAXNAMELEN);
		tmp = sendto(sockFd, &msg, sizeof(msg), 0, addrSo, addrSi);
	}

	return 0;
}

void kaczIo(int sigid) {
	struct sysinfo prywatneInfo;
	struct kpfpMsg incMsg;
	size_t addrSiInc;
	int tmp;
	/*dbg*/printf("kaczIo(%d)\n",sigid);
	tmp = recvfrom(sockFd, &incMsg, sizeof(incMsg), 0, addrSo, &addrSiInc);
	if(!strcmp(incMsg.command, "reply"))
		printf("Wielki Serwer z Zasiedmiogórogrodu odrzekł:\n\tbuf=%s\tname=%s\n\tcommand=%s\n",incMsg.buf, incMsg.name, incMsg.command);
	else if(!strcmp(incMsg.command, "getinfo")) {
		tmp = sysinfo(&prywatneInfo);
		if(tmp == -1)
			myerror("Błąd sysinfo",13);
		strncpy(incMsg.name, name, (uint)MAXNAMELEN);
		snprintf(incMsg.buf, MAXBUFSIZE,
				"\nJa mam nast. problemy:\n\t" \
				"Ilość aktywnych problemów: %d\n\t"\
				"Średnie obciążenie problemami: %lu\n\t"\
				"Ilość spokoju od problemów: %lu\n\t"\
				"Ilość miejsca na problemy: %lu\n",
				prywatneInfo.procs,
				prywatneInfo.loads[2],
				prywatneInfo.freeram,
				prywatneInfo.totalram);
		sendto(sockFd, &incMsg, sizeof(incMsg), 0, addrSo, addrSi);
	}
}
