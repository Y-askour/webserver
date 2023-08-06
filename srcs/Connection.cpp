#include "../include/Connection.hpp"

Connection::Connection(std::vector<Server *> servers,int port)
{
	int yes=1;
	struct sockaddr_in host_addr;
	int listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if (listen_fd == -1)
	{
		perror("webserv(socket)");
	}
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(port);
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int host_addrlen = sizeof(host_addr);

	if (setsockopt(listen_fd,SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) != 0)
	{
		perror("webserv(setsockopt)");
	}
	if (bind(listen_fd,(struct sockaddr *)&host_addr,host_addrlen) != 0)
	{
		perror("webserv(bind)");
	}

	if (fcntl(listen_fd,F_SETFD,O_NONBLOCK) == -1)
	{
		perror("webserv(fcntl)");
	}

	if (listen(listen_fd,255) == -1)
	{
		perror("webserv(listen)");
	}
	this->fd = listen_fd;
	this->port = port;
	this->servers = servers;
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
