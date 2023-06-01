#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/errno.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_CLIENTS 100


int main (int argc, char *argv[])
{
	const char *port = argv[1];
	int numbers_of_clients = 0;
	struct pollfd* fds = (struct pollfd*)malloc(MAX_CLIENTS * sizeof(struct pollfd));
	(void)argc;

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

	int a = 0;
	listen(sockfd,20);
	while (1)
	{
		socklen_t addr_size = sizeof communication;

		int fd_com = accept(sockfd, &communication, &addr_size);
		if (fd_com != -1)
		{
			fds[numbers_of_clients].fd = fd_com;
			fds[numbers_of_clients].events = POLLIN;
			numbers_of_clients++;
			a++;
		}
		std::cout <<  "request number reached : " << a << std::endl;


		char buf[10001];
		int ret = poll(fds,numbers_of_clients,1000);
		if (ret == -1)
		{
			perror("poll");
			return -1;
		}
		else if (!ret)
		{
			std::cerr << "timeout occured" << std::endl;
			continue;
		}
		else
		{
			for (int i = 0;i < numbers_of_clients ;i++)
			{
				std::cout << "***********************************************************" << std::endl;
				std::cout <<  "POLLIN : " << ((fds[i].revents & POLLIN) == POLLIN) << std::endl;
				std::cout <<  "POLLOUT : " << ((fds[i].revents & POLLOUT) == POLLOUT) << std::endl;
				if (fds[i].revents & POLLIN)
				{
					ssize_t s = recv(fds->fd,buf,100000,0);
					std::cout << "bytes readed : "  << s << std::endl;
					buf[s] = 0;
					//std::cout << "req: " << buf << '\n';
					fds[i].events = POLLOUT;
				}
				else if (fds[i].revents & POLLOUT)
				{
					std::string msg = "HTTP/1.1 200 OK\r\n";
					std::string body = "<html>  <body> <h1>test<h1> </body> </html>\r\n";
					msg += "Content-Type: text/html\r\n";
					msg += "Content-Length:" + std::to_string(body.length()) + "\r\n";
					msg += "\r\n";
					msg += body;
					send(fds->fd,msg.c_str(),msg.length(),0);
	 				close(fds[i].fd);
					for (int j = i ; j < numbers_of_clients - 1;j++)
					{
						fds[j] = fds[j + 1];
					}
					numbers_of_clients--;
					std::cout << "response is sent" << std::endl;
				}
				std::cout << "***********************************************************" << std::endl;
			}
			std::cout << "number of clients " << numbers_of_clients << std::endl;
		}
	}
	close(sockfd);
	free(fds);
	return 0;
}
