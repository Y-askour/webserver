#pragma once

#include "Server.hpp"
#include "Parsing.hpp"
#include <iostream>
#include <vector>

class Data : public Parsing
{
	//std::string file;
	std::vector<Server> servers;
	//Parsing	parse;
	public :
		Data();
		Data(std::string name);
		void	parse_file_and_syntax_error(void);
		~Data();
};
