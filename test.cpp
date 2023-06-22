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

void print_struct(void *tst,int size)
{
	struct pollfd *fd = (struct pollfd *) tst;
	int i = 0;
	while (i < size)
	{
		std::cout <<  "fd[" <<  i << "]" <<  fd[i].fd << " " << fd[i].events  << " " << fd[i].revents << std::endl;
		i++;
	}
}

void delete_fd(void *tst,int i,int *nb_of_fd)
{
	struct pollfd *fd = (struct pollfd *) tst;
	close(fd[i].fd);
	for (int j = i;j < *nb_of_fd - 1;j++)
	{
		fd[j].fd =  fd[j + 1].fd;
		fd[j].events =  fd[j + 1].events;
		fd[j].revents =  fd[j + 1].revents;
	}
	fd[*nb_of_fd -1].revents = 0;
	fd[*nb_of_fd -1].fd = 0;
	fd[*nb_of_fd -1].events = 0;
	*nb_of_fd = *nb_of_fd - 1;
}

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
	struct pollfd fds[5];
	memset(fds, 0, sizeof(fds));
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
		//std::cout << "before poll: "  << std::endl;
		//print_struct(fds,5);
		//std::cout << "-----------------" << std::endl;
		int ret = poll(fds,5,0);
		//std::cout << "after poll: "  << std::endl;
		//print_struct(fds,5);
		//std::cout << "-----------------" << std::endl;
		if (ret == -1)
		{
			perror("webserv(poll)");
			return ;
		}
		else
		{
			for (int i = 0; i < nb_of_fds;i++)
			{
				//std::cout << ret << std::endl;
				if (fds[i].revents & POLLERR || fds[i].revents & POLLHUP)
				{
					if (fds[i].revents & POLLERR)
					{
						std::cout << "hey POLLERR: ";
						if (fds[i].revents & POLLNVAL)
						{
							delete_fd(&fds,i, &nb_of_fds);
							std::cout << "POLLNVAL"<< std::endl;
						}
					}
					else if (fds[i].revents & POLLHUP)
					{
						delete_fd(&fds,i, &nb_of_fds);
						std::cout << "hey POLLHUP: " << std::endl;
					}
				}
				else if (fds[i].revents & POLLIN)
				{
					if (fds[i].fd == listen_fd)
					{
						socklen_t addr_size = sizeof communication;
						int fd_client = accept(listen_fd,&communication,&addr_size);
						if (fd_client == -1)
						{
							perror("webserv(accept)");
							return ;
						}
						if (fcntl(fd_client,F_SETFD,O_NONBLOCK) == -1)
						{
							perror("webserv(fcntl)");
							return ;
						}
						fds[nb_of_fds].fd = fd_client;
						fds[nb_of_fds].events = POLLIN;
						nb_of_fds++;
					}
					else
					{
						char buf[1000011];
						ssize_t s = recv(fds[i].fd,buf,1000000,0);
						if (s != -1)

							buf[s] = 0;
						//if (s == -1)
						//{
						//	perror("webserv(recv)");
						//	delete_fd(fds,i,&nb_of_fds);
						//	continue ;
						//}
						//else if (s == 0)
						//{
						//	perror("webserv(recv)");
						//	delete_fd(fds,i,&nb_of_fds);
						//	continue ;
						//}
						fds[i].events = POLLOUT;
					}
				}
				//	delete_fd(fds,i,&nb_of_fds);
				else if (fds[i].revents & POLLOUT)
				{

					if (send(fds[i].fd,msg.c_str(),msg.length(),MSG_OOB) == -1)
					{
						perror("webserv(send)");
						return ;
					}
					delete_fd(fds,i,&nb_of_fds);
					std::cout << "send" << std::endl;
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
