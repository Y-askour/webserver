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
#include <fcntl.h>

int main (int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	std::cout << "--------------- the server has started --------------" << std::endl << std::endl;
	struct addrinfo hints, *res;
	struct sockaddr communication;

	memset(&hints, 0,sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "8080", &hints, &res))
		return 1;

	int sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (bind(sockfd,res->ai_addr,res->ai_addr->sa_len))
		return 2;
	std::cout << "the server waits for a request" << std::endl;
	std::cout << std::endl << std::endl;

	listen(sockfd,20);
	while (1)
	{
		socklen_t addr_size = sizeof communication;
		int fd_com = accept(sockfd, &communication, &addr_size);
		char buf[10000];
		ssize_t s = recv(fd_com,buf,100000,0);
		buf[s] = 0;
		std::cout << "req: " << buf << '\n';
		//std::cout << "the server recv : " << std::endl << std::endl << buf << std::endl;
		std::string msg = "HTTP/1.1 200 OK\r\n";
		msg += "Content-Type: text/html\r\n";
		msg += "Content-Length: 93\r\n";
		msg += "\r\n";
		msg += "<html>  <body> <h1> younes nadi<h1> <h2> hicham nadi bl7e9 younes hssen</h2> </body> </html>\r\n";
		send(fd_com,msg.c_str(),strlen(msg.c_str()),0);
		std::cout << "the server stop" << std::endl;
		shutdown(fd_com, 2);
	}
	return 0;
}
