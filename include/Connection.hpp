#pragma once
#include "Server.hpp"

#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

class Connection
{
	int fd;
	Server& server;
	public:
		Connection(Server &server,int port);
		~Connection();
		int get_fd();
		void get_something_from_server();
};
