#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	int ret;
	char * buff = "Hello from client";
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10088);
    int len = sizeof(buff);
    if (sendto(sockfd, buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *)&server_addr, &len) < 0)
		{
			printf("Failed to send: %s\n", strerror(errno));
			exit(1);
		}
		else
         printf("%ld bytes sent\n", sizeof(buff));
    close(sockfd);

	return 0;
}
