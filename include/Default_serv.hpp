#pragma once

#include <iostream>
#include <vector>
#include <map>

class Default_serv
{
	//could be repeated
	std::vector<int>	listen;
	std::vector<std::string>	index;
	std::vector<std::string>	server_name;
	std::vector<std::pair<int, std::string> > status_page;
	std::vector<std::pair<std::string, std::string> > cgi_info;
	
	//those ones never repeated
	std::string	host;
	std::string root;
	std::string	client_max_body_size;
	int	upload;
	int	auto_index;
	std::string	mime_types_name;
	std::string mime_types;
	std::vector<std::pair<std::string, std::string> > mime_types_parse;
	//std::pair<std::string, std::vector<std::string, std::vector<std::string> > >	mime_types;
	std::vector<std::string>	allow_methods;

	//this one for location
	std::vector<std::pair<int, std::string> > retur;

	public :
		Default_serv(void);
		Default_serv(int);
		//here maybe i will make a function with a string parameter for example "listen"
		//it returns 1 if exists and 0 if not to check if u will work with server var or location
		//function here that put status and path in a pair and adds it to status_page var.
		~Default_serv(void);
};
