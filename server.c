#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define PORT 10088
#define BACKLOG 10


typedef struct {
	char type[5];
	char * data;
	struct sockaddr_in cli_addr;
} heartbeat_t;


int main(int argc, char * argv[])
{
	struct sockaddr_in server_addr;
	int sockfd;
	int ret;
	char buff[96];
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!sockfd)
	{
		printf("Socket creation failed\n");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	heartbeat_t hb;
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
	len = sizeof(hb.cli_addr);
	for (;;)
	{
		fflush(stdout);
		printf("Waiting on port %d\n",PORT);
		n = recvfrom(sockfd, (char *)buff, 128, MSG_WAITALL, &hb.cli_addr,&len);
		if (n != -1)
		{
		printf("Length of: %d\n", len);
		printf("Received bytes: %d\nFrom: %s\n", n,inet_ntoa(hb.cli_addr.sin_addr));
		printf("sizeof n: %ld\n", sizeof(buff));
		if (sizeof(buff[n]) == 1)
		 {
			buff[n] = 0;
			printf("received msg %s\nLength of packet: %ld\n", buff, sizeof(n));
			sleep(10);
		 }
	    }
		else
		{
			printf("Error: %s\n", strerror(errno));
		}

	}
	close(sockfd);
	return 0;
}
