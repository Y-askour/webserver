#include <cstring>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

int main (int argc, char *argv[])
{
	char dst[INET_ADDRSTRLEN];
	struct addrinfo *servinfo;
	struct sockaddr_in use_bind;

	use_bind.sin_family = AF_INET;
	use_bind.sin_port = htons(7777);

	//use_bind.sin_addr.s_addr = inet_addr("10.12.5.4");
	// this code commented is the same as the code above
	use_bind.sin_addr.s_addr =  INADDR_ANY;

	memset(use_bind.sin_zero,0,sizeof(use_bind.sin_zero));

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


			int s = socket(PF_INET,SOCK_STREAM, 0);
			
			if  (bind(s,(struct sockaddr*) &use_bind ,sizeof(use_bind)) == -1)
				return (89);

			close(s);
			std::cout << dst << std::endl;
		}	
	}
	return 0;
}
