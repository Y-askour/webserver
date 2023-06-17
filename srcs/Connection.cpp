#include "../include/Connection.hpp"

Connection::Connection(Server &server,int port): server(server)
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

	if (listen(listen_fd,128) == -1)
	{
		perror("webserv(listen)");
	}
	this->fd = listen_fd;
	//this->server.get_listen();
}

Connection::~Connection()
{
}

void Connection::get_fd()
{
	std::cout << this->fd << std::endl;
}

void Connection::get_something_from_server()
{
	std::cout << this->server.get_root() << std::endl;
	std::cout << "-------" << std::endl;
}

