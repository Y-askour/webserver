#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main()
{
	struct addrinfo hints,*res;

	memset(&hints,0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "9999", &hints, &res))
		return 1;

	int sockfd = socket(AF_INET, SOCK_STREAM,res->ai_protocol);
	if (bind(sockfd,res->ai_addr,res->ai_addrlen))
		return 2;
    return 0;
}
