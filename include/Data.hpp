#pragma once

#include "../include/Server.hpp"
#include "../include/Connection.hpp"
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
	std::map<std::string, std::string>	mime_types_parse;
	std::vector<Connection> connections;
	std::vector<Server*> servers;
	std::vector<Server*>::iterator	servers_itr;
	int nb_clients;
	struct pollfd poll_fd[5];
	public :
		Data();
		Data(const Data & obj);
		~Data();
		void	parse_file_and_syntax_error(void);

		//getters
		std::vector<Server*> &get_servers(void);

		//setters
		void create_listen_sockets();
		void run_server();
		int get_number_of_clients();
};
