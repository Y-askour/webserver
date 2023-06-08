#include "../include/Default_serv.hpp"

//this one for the server
Default_serv::Default_serv(void)
{
	std::cout << "Default_serv default constructer" << std::endl;
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
	this->autoindex = 0;
	//this->mime_types = "~/webserv/config/mime.types"; //this one parse it
	this->allow_methods.push_back("GET");
}

//this one for location
Default_serv::Default_serv(int) {}


int	check_if_number(std::string data)
{
	for (std::string::iterator it = data.begin(); it != data.end(); it++)
	{
		if (!isdigit(*it))
			return (1);
	}
	return (0);
}

void	check_status_code(std::string data)
{
	if (data.compare("200") && data.compare("301") && data.compare("400") \
			&& data.compare("404") && data.compare("405") && data.compare("503") \
			&& data.compare("505"))
		throw ("Error: wrong status code.");
}

void	Default_serv::add_listen(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
	{
		if (check_if_number(data[i]))
			throw ("Error: listen directive only takes number port.");
		if (!(atoi(data[i].c_str()) >= 0 && atoi(data[i].c_str()) <= 65536))
			throw ("Error: port range [0 - 65536].");
		this->listen.push_back(atoi(data[i].c_str()));
	}
	//here check if port valid if number and range and if repeated
}

void	Default_serv::add_index(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
		this->index.push_back(data[i]);
}

void	Default_serv::add_server_name(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
		this->server_name.push_back(data[i]);
}

void	Default_serv::add_status_page(std::vector<std::string> data)
{
	check_status_code(data[0]);
	//maybe here check the path file if exist too
	this->status_page.push_back(std::make_pair(atoi(data[0].c_str()), data[1]));
	//check the status number 
}

void	Default_serv::add_cgi_info(std::vector<std::string> data)
{
	if (data[0].compare(".py") && data[0].compare(".php"))
		throw ("Error: this server only serve .php and .py script.");
	this->cgi_info.push_back(std::make_pair(data[0], data[1]));
	//here check data[0] only .php and .py and check the path
}

void	Default_serv::add_host(std::vector<std::string> data)
{
	this->host = data[0];
	//here check if it's an ip addres by the socket syscall
}

void	Default_serv::add_root(std::vector<std::string> data)
{
	this->root = data[0];
	//maybe here check if that path exist
}

void	Default_serv::add_client_max_body_size(std::vector<std::string> data)
{
	this->client_max_body_size = data[0];
	//if ()
	//check it if the number is valide
}

void	Default_serv::add_upload(std::vector<std::string> data)
{
	if (!data[0].compare("on"))
		this->upload = 1;
	else if (!data[0].compare("off"))
		this->upload = 0;
	else
		throw ("Error: wrong syntax upload only takes on/off.");
}

void	Default_serv::add_autoindex(std::vector<std::string> data)
{
	if (!data[0].compare("on"))
		this->upload = 1;
	else if (!data[0].compare("off"))
		this->upload = 0;
	else
		throw ("Error: wrong syntax autoindex only takes on/off.");
}

void	Default_serv::add_allow_methods(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
	{
		if (data[i].compare("GET") && data[i].compare("POST") && data[i].compare("DELETE"))
			throw ("Error: wrong syntax allow_methods only accept [GET - POST - DELETE].");
		this->allow_methods.push_back(data[i]);
	}
	//here check if methods right POST DELETE GET
}

void	Default_serv::add_return(std::vector<std::string> data)
{
	check_status_code(data[0]);
	//maybe here check the path file if exist too
	this->retur.push_back(std::make_pair(atoi(data[0].c_str()), data[1]));
}

Default_serv::~Default_serv(void) {}
