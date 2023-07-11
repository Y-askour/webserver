#pragma once
#include "../include/Server.hpp"
#include "Connection.hpp"

#include <iostream>

class Request
{
	int fd;
	Server* server;
	public:
		Request(Connection& connection,int fd);
		~Request();
		Server& get_server();
		int get_fd();
};
