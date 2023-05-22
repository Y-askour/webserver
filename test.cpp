#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>

int main (int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	struct addrinfo *servinfo;
	if (getaddrinfo("facebook.com", "http", NULL,&servinfo))
		return 1;
	std::cout << servinfo->ai_family << std::endl;
	return 0;
}
