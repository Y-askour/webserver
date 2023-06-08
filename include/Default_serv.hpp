#pragma once

//#include "Parsing.hpp"
#include <iostream>
#include <fstream>
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
	int	autoindex;
	//std::string	mime_types_name;
	std::string mime_types;
	std::vector<std::pair<std::string, std::string> > mime_types_parse;
	//std::pair<std::string, std::vector<std::string, std::vector<std::string> > >	mime_types;
	std::vector<std::string>	allow_methods;

	//this one for location
	std::vector<std::pair<int, std::string> > retur;

	public :
		Default_serv(void);
		Default_serv(int);
		//setters
		void	add_listen(std::vector<std::string> data);
		void	add_index(std::vector<std::string> data);
		void	add_server_name(std::vector<std::string> data);
		void	add_status_page(std::vector<std::string> data);
		void	add_cgi_info(std::vector<std::string> data);
		void	add_host(std::vector<std::string> data);
		void	add_root(std::vector<std::string> data);
		void	add_client_max_body_size(std::vector<std::string> data);
		void	add_upload(std::vector<std::string> data);
		void	add_autoindex(std::vector<std::string> data);
		void	add_mime_types(std::vector<std::string> data);
		void	add_allow_methods(std::vector<std::string> data);
		void	add_return(std::vector<std::string> data);


		//parsing it

		//getters
		std::string get_mime_types(void)
		{
			return (this->mime_types);
		}


		//here maybe i will make a function with a string parameter for example "listen"
		//it returns 1 if exists and 0 if not to check if u will work with server var or location
		//function here that put status and path in a pair and adds it to status_page var.
		~Default_serv(void);
};
