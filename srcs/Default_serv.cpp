#include "../include/Default_serv.hpp"

//this one for the server
Default_serv::Default_serv(void)
{
	//here init all variables
	this->listen.push_back(80);
	this->index.push_back("index.html");
	this->server_name.push_back("localhost");
	//
		//not now i have to know what kind of error we will handle i will make a function that adds error
		//here a loop to push all status pages with their id status
		this->status_page.push_back(std::make_pair(404, "~/webserv/status_page/404.html"));
	//
	//cgi info
	this->cgi_info.push_back(std::make_pair(".py", "/usr/bin/python3"));
	this->cgi_info.push_back(std::make_pair(".php", "/usr/bin/php"));
	//
	this->host = "127.0.0.1";
	this->root = "~/webserv/www";
	this->client_max_body_size = "10000";
	this->upload = 0;
	this->auto_index = 0;
	this->mime_types = "~/webserv/config/mime.types"; //this one parse it
	this->allow_methods.push_back("GET");
}

//this one for location
Default_serv::Default_serv(int) {}

void	Default_serv::add_listen(std::vector<std::string> data)
{
	this->listen.push_back(80);
	return ;
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
		this->listen.push_back(atoi(data[i].c_str()));
}

void	Default_serv::add_index(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_server_name(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_status_page(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_cgi_info(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_host(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_root(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_client_max_body_size(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_upload(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_auto_index(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_mime_types(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_allow_methods(std::vector<std::string> data)
{
	(void)data;
}

void	Default_serv::add_return(std::vector<std::string> data)
{
	(void)data;
}

Default_serv::~Default_serv(void) {}
