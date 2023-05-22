#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>

int main (int argc, char *argv[])
{
	char dst[INET_ADDRSTRLEN];
	struct addrinfo *servinfo;

	if (argc != 2)
		return 1;

	if (getaddrinfo(argv[1], "http", NULL,&servinfo))
		return 1;

	for (;servinfo != NULL; servinfo = servinfo->ai_next)
	{
		if (servinfo->ai_family == AF_INET)
		{
			struct sockaddr_in *ip = (struct sockaddr_in*) servinfo->ai_addr;
			void *addr = &(ip->sin_addr);
			inet_ntop(servinfo->ai_family,addr,dst,sizeof dst);
			std::cout << dst << std::endl;
		}	
	}
	freeaddrinfo(servinfo);
	return 0;
}
