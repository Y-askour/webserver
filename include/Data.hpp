#pragma once

#include "../include/Server.hpp"
#include "../include/Connection.hpp"
#include "../include/Request.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <unistd.h>



class Data
{
	protected :
	size_t check;

	std::map<std::string, std::string>	mime_types_parse;

	// all fds that are listening
	std::vector<Connection> connections;

	// all servers with all their information from the config
	std::vector<Server*> servers;

	std::vector<Server*>::iterator	servers_itr;


	// all mantained fds
	std::vector<struct pollfd> test;


	std::vector<Request> req_res;


	public :
		Data();
		Data(const Data & obj);
		~Data();

		//getters
		std::vector<Server*> &get_servers(void);

		//setters
		void create_listen_sockets();
		void run_server();
		Connection *get_connection_by_fd(int fd);
		Request *get_request_by_fd(int fd);
		void delete_request(int fd);
		bool is_a_connection(int fd);
		int check_is_headers_done(std::string request);
};
