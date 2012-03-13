#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {

	struct sockaddr_in serv_addr;
	int fd;
	char buf[10];

	fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(fd != -1);
	
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");
	serv_addr.sin_port			= htons(12346);

	connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	strcpy(buf, "lol");
	send(fd, buf, strlen(buf), 0);

	close(fd);
	return 0;
}

