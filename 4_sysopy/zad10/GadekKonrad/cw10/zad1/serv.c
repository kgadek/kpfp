#include <stddef.h>
#include "common.h"

#define MAXCLIENTS 10

int main(int argc, char **argv) {
	int sockFd;
	int sockNetFd;
	int sockCurr;
	int mode;
	int i;
	int tmp;
	struct kpfpClEntry clts[MAXCLIENTS];
	struct kpfpMsg msg;
	size_t addrSi;
	struct sockaddr* addrSo;
	struct sockaddr_un svAddr;
	struct sockaddr_un clAddr;
	struct sockaddr_in svNetAddr;
	struct sockaddr_in clNetAddr;
	struct pollfd fds[2];
	struct timespec timeout;
	char nameA[MAXNAMELEN];
	char nameB[MAXNAMELEN];

	/*init*/
	if(argc != 3)
		myerror("Błędna ilość argumentów -- podaj:\n\tserv port sciezka_socket",1);
	for(i=0;i<MAXCLIENTS; ++i)
		clts[i].name[0]=0;

	sockFd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if(sockFd == -1)
		myerror("Błąd socket",2);
	svAddr.sun_family = AF_UNIX;
	strncpy(svAddr.sun_path, argv[2], 108-1);

	sockNetFd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sockNetFd == -1)
		myerror("Błąd socket",3);
	svNetAddr.sin_family = AF_INET;
	svNetAddr.sin_port = htons((uint16_t)atoi(argv[i]));
	svNetAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	timeout.tv_sec = timeout.tv_nsec = -1;

	tmp = bind(sockFd, (struct sockaddr*)&svAddr, SUN_LEN(&svAddr)); /* !!! */
	if(tmp == -1)
		myerror("Błąd bind!",4);
	tmp = bind(sockNetFd, (struct sockaddr*)&svNetAddr, sizeof(svNetAddr));
	if(tmp == -1)
		myerror("Błąd bind!",5);

	tmp = 1;
	tmp = setsockopt(sockNetFd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
	if(tmp == -1)
		myerror("Błąd setsockopt -- SO_REUSEADDR",6);

	fds[0].fd = sockFd;
	fds[1].fd = sockNetFd;
	fds[0].events = fds[1].events = POLLIN;
	for(;;) { /*main fuhrer-loop*/
		poll(fds, 2, -1);
		if((fds[0].revents & POLLIN) == POLLIN) {
			printf("Lokalny ziooooooooom!");
			addrSi = sizeof(clAddr);
			addrSo = (struct sockaddr*)&clAddr;
			sockCurr = sockFd;
			mode = 0; /*UNIX*/
		} else if((fds[1].revents & POLLIN) == POLLIN) {
			printf("Doch!... Kolejny miastowy...\n");
			addrSi = sizeof(clNetAddr);
			addrSo = (struct sockaddr*)&clNetAddr;
			sockCurr = sockNetFd;
			mode = 1; /*INET*/
		} else
			myerror("Że kto? Co? Yyy eee ja nie rozumiem, nie umiem.\n", 7);

		tmp = recvfrom(sockCurr, &msg, sizeof(msg), 0, addrSo, &addrSi);
		if(tmp == -1)
			myerror("Bląd recvfrom",8);
		if(!strcmp(msg.command, "logout")) {
			printf("Bajbaj, maszkaro!\n");
			for(i=0; i<MAXCLIENTS; ++i) {
				if(clts[i].name[0] == 0)
					continue;
				if(!strcmp(msg.name, clts[i].name))
					break;
			}
			if(i<MAXCLIENTS) {
				free(clts[i].addrSo);
				clts[i].name[0] = 0;
				strncpy(msg.buf, "Okejka, bajbaj", MAXBUFSIZE);
			} else
				strncpy(msg.buf, "You were not logged", MAXBUFSIZE);
		} else if(!strcmp(msg.command, "ask")) {
			printf("Ktoś pyta czy ktoś ma problem\n");
			strncpy(nameA, msg.name, MAXNAMELEN);
			strncpy(nameB, msg.buf, MAXNAMELEN);
			for(i=0;i<MAXCLIENTS; ++i) {
				if(clts[i].name[0] == 0)
					continue;
				if(!strcmp(nameB, clts[i].name))
					break;
			}
		} else if(!strcmp(msg.command, "list")) {
		} else if(!strcmp(msg.command, "registerMe")) {
		} else
			strncpy(msg.buf, "WTF? invalid command!",MAXBUFSIZE);
		strncpy(msg.command, "reply", MAXCOMMANDSIZE);
		tmp = sendto(sockCurr, &msg, sizeof(msg), 0, addrSo, addrSi);
		if(tmp == -1)
			myerror("Błąd sendto",9);
	}

	return 0;
}
