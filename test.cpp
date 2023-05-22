#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

int main (int argc, char *argv[])
{
	struct addrinfo hints, *res;
	char ipstr[INET_ADDRSTRLEN];

	if (argc != 2)
		return -1;
	memset(&hints, 0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(argv[1], "http", &hints, &res))
		return 2;

	void *addr;
	if (res->ai_family == AF_INET)
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
		addr = &(ipv4->sin_addr);
	}
	inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
	std::cout << "connecting to " << argv[1] << " with ip : " << ipstr << std::endl;

	int sockefd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if (connect(sockefd, res->ai_addr, res->ai_addrlen))
		return 3;
	while(1);

	return 0;
}
