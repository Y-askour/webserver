#pragma once
#include "../include/Server.hpp"
#include "Connection.hpp"

#include <iostream>

class Request
{
	int fd;
	Server* server;
	std::string request_buf;
	size_t n_bytes;

	std::string request_line;
	std::map<std::string, std::string> headers;
	std::string body;
	
	public:
		Request(Connection& connection,int fd);
		~Request();
		Server& get_server();
		int get_fd();
		void set_request_buf(char *buf);
		void set_n_bytes(size_t n);
		void split_by_rclt();
		void remove_spaces(std::string &t);
};
