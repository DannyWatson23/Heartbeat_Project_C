#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define PORT 10088
#define BACKLOG 10


struct {
	char msg[50];
	char * receiver;
}heartbeat_t;


int main(int argc, char * argv[])
{
	struct sockaddr_in server_addr, cli_addr;
	int sockfd;
	int ret;
	char buff[96];
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (!sockfd)
	{
		printf("Socket creation failed\n");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	ret = bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret != 0)
	{
		printf("Bind failed: %d\n", ret);
		switch(errno)
		{
		case EADDRINUSE:
			printf("Error address already in use\n");
			exit(1);
		case EINVAL:
			printf("Error, socket already bound to an address\n");
			exit(1);
		default:
			printf("Unexpected error has occurred: %s\n", strerror(errno));
			exit(1);
		}
	}
	int n;
	int len;
	len = sizeof(cli_addr);
	for (;;)
	{
		printf("Waiting on port %d\n",PORT);
		n = recvfrom(sockfd, (char *)buff, 128, MSG_WAITALL, (struct sockaddr *)&cli_addr,&len);
		printf("Received bytes: %d\n", n);
		if (sizeof(buff[n]) > 1)
		{
			buff[n] = 0;
			printf("received msg %s\nLength of packet: %ld", buff, sizeof(buff));
		}
	}

	close(sockfd);
	return 0;
}
