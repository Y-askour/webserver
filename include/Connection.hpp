#pragma once
#include "Server.hpp"
#include "Data.hpp"

class Connection
{
	int fd;
	Server& server;
	public:
		Connection(Server &server);
		~Connection();
};
