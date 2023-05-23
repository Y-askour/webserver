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
	(void)argc;
	(void)argv;

	std::cout << "the server has started" << std::endl;
	struct addrinfo hints, *res;
	struct sockaddr communication;

	memset(&hints, 0,sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "8080", &hints, &res))
		return 1;

	int sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if (bind(sockfd,res->ai_addr,res->ai_addr->sa_len))
		return 2;
	std::cout << "the server waits for a request" << std::endl;

	listen(sockfd,20);
	socklen_t addr_size = sizeof communication;
	int fd_com = accept(sockfd, &communication, &addr_size);
	void *buf = NULL;
	recv(fd_com,buf,100000,0);
	std::cout << "the server recv : " << buf << std::endl;
	send(fd_com,"hey user",8,0);
	shutdown(fd_com,2);
	close(fd_com);
	std::cout << " the server stop" << std::endl;

	while(1);

	return 0;
}
