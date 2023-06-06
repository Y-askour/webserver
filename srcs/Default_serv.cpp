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

Default_serv::~Default_serv(void) {}
