#include <cstdio>
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

int create_connection(int port)
{
	int yes=1;
	struct sockaddr_in host_addr;
	int listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if (listen_fd == -1)
	{
		perror("webserv(socket)");
		return -1;
	}
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(port);
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int host_addrlen = sizeof(host_addr);

	if (setsockopt(listen_fd,SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) != 0)
	{
		perror("webserv(setsockopt)");
		return -1;
	}
	if (bind(listen_fd,(struct sockaddr *)&host_addr,host_addrlen) != 0)
	{
		perror("webserv(bind)");
		return -1;
	}

	if (fcntl(listen_fd,F_SETFD,O_NONBLOCK) == -1)
	{
		perror("webserv(fcntl)");
		return -1;
	}

	if (listen(listen_fd,128) == -1)
	{
		perror("webserv(listen)");
		return -1;

	}
	return listen_fd;
}

void run_server(int listen_fd)
{
	int nb_of_fds = 0;
	struct pollfd fds[1024];
	fds[nb_of_fds].fd = listen_fd;
	fds[nb_of_fds].events = POLLIN;
	nb_of_fds++;
	struct sockaddr communication;

	std::ifstream f("./younes/www/html/index.html");
	std::string body = "<html>  <body> <h1>test<h1> </body> </html>\r\n";
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf();
		body = ss.str();
	}
	std::string msg = "HTTP/1.1 200 OK\r\n";
	msg += "Content-Type: text/html\r\n";
	msg += "Content-Length:" + std::to_string(body.length()) + "\r\n";
	msg += "\r\n";
	msg += body;

	while (1)
	{
		int ret = poll(fds,nb_of_fds,2000);
		if (ret == -1)
		{
			perror("webserv(poll)");
			return ;
		}
		else if (!ret)
		{
			//std::cout <<  "timemout occured" << "  number of fds : " << nb_of_fds << std::endl; 
			continue;
		}
		else
		{
			for (int i = 0; i < nb_of_fds;i++)
			{
				if (fds[i].revents & POLLIN)
				{
					//std::cout << "pollin" << std::endl;
					if (fds[i].fd == listen_fd)
					{
						socklen_t addr_size = sizeof communication;
						int fd_client = accept(listen_fd,&communication,&addr_size);
						if (fd_client == -1)
						{
							perror("webserv(accept)");
							return ;
						}


						char buf[1000011];
						ssize_t s = recv(fd_client,buf,1000000,0);
						buf[s] = 0;

						// add the client fd to struct
						fds[nb_of_fds].fd = fd_client;
						fds[nb_of_fds].events = POLLOUT;
						nb_of_fds++;
					}
				}

				if (fds[i].revents & POLLOUT)
				{
					//std::cout << "pollout" << std::endl;

					if (send(fds[i].fd,msg.c_str(),msg.length(),MSG_OOB) == -1)
					{
						//std::cout << fds[i].fd << std::endl;
						perror("webserv(send)");
						return ;
					}
					//std::cout << "sended : " << fds[i].fd  << " number of waiting fds : " << nb_of_fds << std::endl;

					close(fds[i].fd);
					for (int j = i;j < nb_of_fds - 1;j++)
					{
						fds[j].fd =  fds[j + 1].fd;
						fds[j].events =  fds[j + 1].events;
						fds[j].revents =  fds[j + 1].revents;
					}
					fds[nb_of_fds -1].revents = 0;
					fds[nb_of_fds -1].fd = 0;
					fds[nb_of_fds -1].events = 0;
					nb_of_fds--;
				}

			}
		}
	}
}

int main (int argc, char *argv[])
{
	struct pollfd fds[1024];
	int listen_fd;

	if (argc == 2)
		listen_fd = create_connection(atoi(argv[1]));
	else
		listen_fd = create_connection(8080);

	if (listen_fd > 0)
	{
		run_server(listen_fd);
		close(listen_fd);
	}
	return 0;
}
