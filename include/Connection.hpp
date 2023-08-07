#pragma once
#include "Server.hpp"

#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

class Connection
{
	int fd;
	int port;
	std::vector<Server *> servers;
	public:
		Connection(std::vector<Server *>,std::pair<int,std::string>);
		~Connection();
		int get_fd();
		std::vector<Server *> get_servers();
		int get_port();
};
