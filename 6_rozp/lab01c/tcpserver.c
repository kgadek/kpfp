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

	struct sockaddr_in serv_addr, cli_addr;
	int fd, cli_fd, cli_len;
	char buf[10];

	fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(fd != -1);
	
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serv_addr.sin_port			= htons(12346);

	bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(fd, 5);
	while(1) {
		cli_fd = accept(fd, (struct sockaddr*)&cli_addr, &cli_len);
		recv(cli_fd, buf, sizeof(buf), MSG_WAITALL);
		printf("buf = %s\n", buf);
		close(cli_fd);
	}

	close(fd);

	return 0;
}

