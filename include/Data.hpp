#pragma once

#include "../include/Server.hpp"
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
	std::vector<Server*> servers;
	std::vector<Server*>::iterator	servers_itr;
	public :
		Data();
		Data(const Data & obj);
		Data	& operator=(const Data & obj);
		//getters
		std::vector<Server*> &get_servers(void);
		//setters
		~Data();
};
