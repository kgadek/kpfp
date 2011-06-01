#define _GNU_SOURCE
#include "common.h"

void showClMsg(clMsg *msg, const char *header) {
	printf("%s\n",header?header:"Wiadomość klienta:");
	printf("\ttype = %ld\n",msg->type);
	printf("\tmyQueueNum = %d\n",msg->myQueueNum);
	printf("\tmyNameIs = %s\n",msg->myNameIs);
	printf("\tiWantToSay = %s\n",msg->iWantToSay);
}

void showSrvMsg(svMsg *msg, const char *header) {
	printf("%s\n", header?header:"Wiadomość serwera:");
	printf("\ttype = %ld\n",msg->type);
	printf("\thisNameIs = %s\n",msg->hisNameIs);
	printf("\theSaid = %s\n",msg->heSaid);
	printf("\tmsgInQueue = %d\n",(int)msg->msgInQueue);
	printf("\tbytesInQueue = %ld\n",msg->bytesInQueue);
	printf("\tmaxBytesInQueue = %ld\n",msg->maxBytesInQueue);
	printf("\tpidReader = %d\n",msg->pidReader);
	printf("\tpidSender = %d\n",msg->pidSender);
	printf("\tlastRead = %d\n",(int)msg->lastRead);
	printf("\tlastSend = %d\n",(int)msg->lastSend);
}
