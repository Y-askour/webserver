#pragma once

#include "../include/Server.hpp"
#include "../include/Connection.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>

class Data
{
	protected :
	std::map<std::string, std::string>	mime_types_parse;
	std::vector<Connection> connections;
	std::vector<Server*> servers;
	std::vector<Server*>::iterator	servers_itr;
	public :
		Data();
		Data(const Data & obj);
		void	parse_file_and_syntax_error(void);
		//getters
		std::vector<Server*> &get_servers(void);
		void create_listen_sockets();
		//setters
		~Data();
};
