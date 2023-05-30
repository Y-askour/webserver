#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
const char port[] = "8080";

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

	if (getaddrinfo(NULL, port , &hints, &res))
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
		if (fd_com == -1)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				continue;
		}
		char buf[10000];
		ssize_t s = recv(fd_com,buf,100000,0);
		std::cout << "bytes readed : "  << s << std::endl;
		buf[s] = 0;
		std::cout << "req: " << buf << '\n';
		std::string msg = "HTTP/1.1 200 OK\r\n";
		std::string body = "<html>  <body> <h1>test<h1> </body> </html>\r\n";
		msg += "Content-Type: text/html\r\n";
		msg += "Content-Length:" + std::to_string(body.length()) + "\r\n";
		msg += "\r\n";
		msg += body;
		send(fd_com,msg.c_str(),msg.length(),0);
		std::cout << "the server stop" << std::endl;
		close(fd_com);
	}
	return 0;
}
