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
		std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void);
		//std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void);
		void	parse_file_and_syntax_error(void);
		~Data();
};
