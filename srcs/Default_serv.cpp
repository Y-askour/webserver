#include "../include/Default_serv.hpp"

//this one for the server
Default_serv::Default_serv(void)
{
	//std::cout << "Default_serv default constructer" << std::endl;
	//here init all variables
	this->listen.push_back(8080);
	this->index.push_back("index.html");
	this->server_name.push_back("localhost");
	//
		//not now i have to know what kind of error we will handle i will make a function that adds error
		//here a loop to push all status pages with their id status
		this->status_page.push_back(std::make_pair(200, "status_page/200.html"));
		this->status_page.push_back(std::make_pair(301, "status_page/301.html"));
		this->status_page.push_back(std::make_pair(400, "status_page/400.html"));
		this->status_page.push_back(std::make_pair(404, "status_page/404.html"));
		this->status_page.push_back(std::make_pair(405, "status_page/405.html"));
		this->status_page.push_back(std::make_pair(503, "status_page/503.html"));
		this->status_page.push_back(std::make_pair(505, "status_page/505.html"));
	//
	//cgi info
	//maybe this one should not be added until it give by the user
	this->cgi_info.push_back(std::make_pair(".py", "/usr/bin/python3"));
	this->cgi_info.push_back(std::make_pair(".php", "/usr/bin/php"));
	//
	this->host = "127.0.0.1";
	this->root = "var/www/";
	this->client_max_body_size = "10000";
	this->upload = 0;
	this->autoindex = 0;
	//this->mime_types = "~/webserv/config/mime.types"; //this one parse it
	this->allow_methods.push_back("GET");
}

//this one for location
Default_serv::Default_serv(int)
{
	std::cout << "Default_serv int constructer" << std::endl;
}

void	Default_serv::take_off_default_setup(void)
{
	if (this->listen.size() > 1)
		this->listen.erase(this->listen.begin());
	if (this->index.size() > 1)
		this->index.erase(this->index.begin());
	if (this->server_name.size() > 1)
		this->server_name.erase(this->server_name.begin());
	//if (this->status_page.size() > 1)
	//	this->status_page.erase(this->status_page.begin());
	if (this->allow_methods.size() > 1)
		this->allow_methods.erase(this->allow_methods.begin());
}

void	Default_serv::check_server_setup_duplicate(void)
{
	for (int	i = 0; i < static_cast<int>(this->listen.size()); i++)
	{
		if (std::count(this->listen.begin(), this->listen.end(), this->listen[i]) > 1)
			throw ("Error: listen have a repeated post.");
	}

	//if (this->)
	//for (int	i = 0; i < static_cast<int>(this->listen.size()); i++)
	//{
	//	if (std::count(this->listen.begin(), this->listen.end(), this->listen[i]) > 1)
	//		throw ("Error: listen have a repeated post.");
	//}
}

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
	if (check_if_number(data[0]))
		throw ("Error: listen directive only takes number port.");
	if (!(atoi(data[0].c_str()) >= 0 && atoi(data[0].c_str()) <= 65536))
		throw ("Error: port range [0 - 65536].");
	this->listen.push_back(atoi(data[0].c_str()));
	//for (int	i = 0; i < static_cast<int>(data.size()); i++)
	//{
	//	if (check_if_number(data[i]))
	//		throw ("Error: listen directive only takes number port.");
	//	if (!(atoi(data[i].c_str()) >= 0 && atoi(data[i].c_str()) <= 65536))
	//		throw ("Error: port range [0 - 65536].");
	//	this->listen.push_back(atoi(data[i].c_str()));
	//}
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
	if (atoi(data[0].c_str()) == 200)
		this->retur[0].second = data[1];
	else if (atoi(data[1].c_str()) == 301)
		this->retur[1].second = data[1];
	else if (atoi(data[2].c_str()) == 400)
		this->retur[2].second = data[1];
	else if (atoi(data[3].c_str()) == 404)
		this->retur[3].second = data[1];
	else if (atoi(data[4].c_str()) == 405)
		this->retur[4].second = data[1];
	else if (atoi(data[5].c_str()) == 503)
		this->retur[5].second = data[1];
	else if (atoi(data[6].c_str()) == 505)
		this->retur[6].second = data[1];
	//maybe here check the path file if exist too
	//this->status_page.push_back(std::make_pair(atoi(data[0].c_str()), data[1]));
	//check the status number 
}

void	Default_serv::add_cgi_info(std::vector<std::string> data)
{
	if (data[0].compare(".py") && data[0].compare(".php"))
		throw ("Error: this server only serve .php and .py script.");
	if (!data[0].compare(".py"))
		this->cgi_info[0].second = data[1];
	else if (!data[0].compare(".php"))
		this->cgi_info[1].second = data[1];

	//this->cgi_info.push_back(std::make_pair(data[0], data[1]));
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

	for (int	i = 0; i < static_cast<int>(this->retur.size()); i++)
	{
		if (this->retur[i].first == atoi(data[0].c_str()))
		{
			this->retur[i].second = data[1];
			return ;
		}
	}
	this->retur.push_back(std::make_pair(atoi(data[0].c_str()), data[1]));
}

Default_serv::~Default_serv(void)
{
	//std::cout << "Default_serv default destructer." << std::endl;
}
