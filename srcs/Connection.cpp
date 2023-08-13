#include "../include/Connection.hpp"
#include <string>

Connection::Connection(std::vector<Server *> servers,std::pair<int,std::string> host_port)
{
	int yes=1;
	this->fd  = -1;
	struct sockaddr_in host_addr;
	int listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if (listen_fd == -1)
	{
		perror("webserv(socket)");
	}
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(host_port.first);
	host_addr.sin_addr.s_addr = inet_addr(host_port.second.c_str());
	int host_addrlen = sizeof(host_addr);

	if (setsockopt(listen_fd,SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) != 0)
	{
		perror("webserv(setsockopt)");
		return ;
	}
	if (bind(listen_fd,(struct sockaddr *)&host_addr,host_addrlen) != 0)
	{
		perror("webserv(bind)");
		return ;
	}

	if (fcntl(listen_fd,F_SETFD,O_NONBLOCK) == -1)
	{
		perror("webserv(fcntl)");
		return ;
	}
	if (listen(listen_fd,255) == -1)
	{
		perror("webserv(listen)");
		return ;
	}
	this->fd = listen_fd;
	this->port = host_port.first;
	this->servers = servers;
	this->ip = host_port.second;
}

Connection::~Connection()
{
}

int Connection::get_fd()
{
	return this->fd;
}

std::vector<Server*> Connection::get_servers()
{
	return this->servers;
}
int Connection::get_port()
{
	return this->port;
}

std::string Connection::get_ip()
{
	return this->ip;
}
