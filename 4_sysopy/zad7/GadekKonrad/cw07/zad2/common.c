#define _GNU_SOURCE
#include "common.h"

void showClMsg(clMsg *msg, const char *header) {
	printf("%s\n",header?header:"Wiadomość klienta:");
	printf("\ttype = %ld\n",msg->type);
	printf("\tmyNameIs = %s\n",msg->myNameIs);
	printf("\tmyCard = %d\n",msg->myCard);
	printf("\tiWantToSay = %s\n",msg->iWantToSay);
}

void showSrvMsg(svMsg *msg, const char *header) {
	printf("%s\n", header?header:"Wiadomość serwera:");
	printf("\ttype = %ld\n",msg->type);
	printf("\thisNameIs = %s\n",msg->hisNameIs);
	printf("\theSaid = %s\n",msg->heSaid);
	printf("\tLS = %d\n",(int)msg->LS);
	printf("\tmsgSent = %d\n",(int)msg->msgSent);
}

void hr() {
	printf("--------------------------------------------------------------------------------\n");
}

char getRandomChar() {
	char res = (char)(rand()%('z'-6-'A')+'A');
	if(res > 'Z')
		res = (char)(res + 6); /* pomijam znaki z poza a-zA-Z */
	return res;
}

void makeMeQuit(int sigId) {
	printf("\nSygnał %d prosi o zamknięcie... Nie dam się prosić...\n",sigId);
	exit(0);
}
