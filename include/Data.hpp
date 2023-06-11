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
	//std::vector<std::pair<std::string, std::string> > mime_types_parse;
	std::map<std::string, std::string>	mime_types_parse;
	std::vector<Server> servers;
	public :
		Data();
		Data(const Data & obj);
		Data(std::string name);
		//void	print_config(void);
		//std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void);

		//std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void);
		void	parse_file_and_syntax_error(void);
		~Data();
};
