#include "../include/Default_serv.hpp"

//this one for the server
Default_serv::Default_serv(void)
{
	//std::cout << "Defautl_serv constructer" << std::endl;
	this->listen.push_back(8080);
	this->index.push_back("index.html");
	this->server_name.push_back("localhost");
	this->status_page.push_back(std::make_pair(200, "status_page/200.html"));
	this->status_page.push_back(std::make_pair(301, "status_page/301.html"));
	this->status_page.push_back(std::make_pair(400, "status_page/400.html"));
	this->status_page.push_back(std::make_pair(404, "status_page/404.html"));
	this->status_page.push_back(std::make_pair(405, "status_page/405.html"));
	this->status_page.push_back(std::make_pair(503, "status_page/503.html"));
	this->status_page.push_back(std::make_pair(505, "status_page/505.html"));
	this->host = "127.0.0.1";
	this->root = "var/www/";
	this->client_max_body_size = "10000";
	this->upload = 0;
	this->autoindex = 0;
	this->allow_methods.push_back("GET");
}

//this one for location
Default_serv::Default_serv(int) {
	//std::cout << "Defautl_serv for location constructer" << std::endl;
}

Default_serv::Default_serv(const Default_serv & obj)
{
	*this = obj;
}

Default_serv & Default_serv::operator=(const Default_serv & obj)
{
	this->listen = obj.listen;
	this->index = obj.index;
	this->server_name = obj.server_name;
	this->status_page = obj.status_page;
	this->cgi_info = obj.cgi_info;
	this->host = obj.host;
	this->root = obj.root;
	this->client_max_body_size = obj.client_max_body_size;
	this->upload = obj.upload;
	this->autoindex = obj.autoindex;
	this->allow_methods = obj.allow_methods;
	this->retur = obj.retur;
	return (*this);
}

//parsing
void	Default_serv::take_off_default_setup(void)
{
	if (this->listen.size() > 1)
		this->listen.erase(this->listen.begin());
	if (this->index.size() > 1)
		this->index.erase(this->index.begin());
	if (this->server_name.size() > 1)
		this->server_name.erase(this->server_name.begin());
	if (this->allow_methods.size() > 1)
		this->allow_methods.erase(this->allow_methods.begin());
}

void	Default_serv::check_server_setup_duplicate(std::vector<int> &port_checking)
{
	for (int	i = 0; i < static_cast<int>(this->listen.size()); i++)
	{
		port_checking.push_back(this->listen[i]);
		if (std::count(this->listen.begin(), this->listen.end(), this->listen[i]) > 1)
			throw ("Error: listen have a repeated post.");
	}
	for (int	i = 0; i < static_cast<int>(this->allow_methods.size()); i++)
	{
		if (std::count(this->allow_methods.begin(), this->allow_methods.end(), this->allow_methods[i]) > 1)
			throw ("Error: allow_methods have a duplicate method.");
	}
	for (int	i = 0; i < static_cast<int>(this->index.size()); i++)
	{
		if (std::count(this->index.begin(), this->index.end(), this->index[i]) > 1)
			throw ("Error: index have a duplicate file.");
	}
	for (int	i = 0; i < static_cast<int>(this->server_name.size()); i++)
	{
		if (std::count(this->server_name.begin(), this->server_name.end(), this->server_name[i]) > 1)
			throw ("Error: server_name have a duplicate server_name.");
	}
}

int	Default_serv::check_if_number(std::string data)
{
	for (std::string::iterator it = data.begin(); it != data.end(); it++)
	{
		if (!isdigit(*it))
			return (1);
	}
	return (0);
}

void	Default_serv::check_status_code(std::string data)
{
	if (data.compare("200") && data.compare("301") && data.compare("400") \
			&& data.compare("404") && data.compare("405") && data.compare("503") \
			&& data.compare("505"))
		throw ("Error: wrong status code, allow code [200 - 301 - 400 - 404 - 405 - 503 - 505].");
}

//setters
void	Default_serv::set_listen(std::vector<std::string> data)
{
	if (check_if_number(data[0]))
		throw ("Error: listen directive only takes number port.");
	if (!(atoi(data[0].c_str()) >= 0 && atoi(data[0].c_str()) <= 65536))
		throw ("Error: port range [0 - 65536].");
	this->listen.push_back(atoi(data[0].c_str()));
}

void	Default_serv::set_index(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
		this->index.push_back(data[i]);
}

void	Default_serv::set_server_name(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
		this->server_name.push_back(data[i]);
}

void	Default_serv::set_status_page(std::vector<std::string> data)
{
	check_status_code(data[0]);
	if (atoi(data[0].c_str()) == 200)
		this->status_page[0].second = data[1];
	else if (atoi(data[0].c_str()) == 301)
		this->status_page[1].second = data[1];
	else if (atoi(data[0].c_str()) == 400)
		this->status_page[2].second = data[1];
	else if (atoi(data[0].c_str()) == 404)
		this->status_page[3].second = data[1];
	else if (atoi(data[0].c_str()) == 405)
		this->status_page[4].second = data[1];
	else if (atoi(data[0].c_str()) == 503)
		this->status_page[5].second = data[1];
	else if (atoi(data[0].c_str()) == 505)
		this->status_page[6].second = data[1];
	//maybe here check the path file if exist too
}

void	Default_serv::set_cgi_info(std::vector<std::string> data)
{
	if (data[0].compare(".py") && data[0].compare(".php"))
		throw ("Error: this server only serve .php and .py script.");
	for (int i = 0; i < static_cast<int>(this->cgi_info.size()); i++)
	{
		if (!this->cgi_info[i].first.compare(data[0]))
			throw ("Error: you have a duplicated cgi_info.");
	}
	this->cgi_info.push_back(std::make_pair(data[0], data[1]));
}

void	Default_serv::set_host(std::vector<std::string> data)
{
	this->host = data[0];
	//here check if it's an ip addres by the socket syscall
}

void	Default_serv::set_root(std::vector<std::string> data)
{
	this->root = data[0];
	DIR *dir;

	if (!(dir = opendir(this->root.c_str())))
		throw ("Error: root directory not exists.");
  closedir(dir);
	//maybe here keep the dir fd to use it in the execution
}

void	Default_serv::set_client_max_body_size(std::vector<std::string> data)
{
	if (check_if_number(data[0]))
		throw ("Error: client_max_body_size only takes digit.");
	this->client_max_body_size = data[0];
	//check it if the number is valide
}

void	Default_serv::set_upload(std::vector<std::string> data)
{
	if (!data[0].compare("on"))
		this->upload = 1;
	else if (!data[0].compare("off"))
		this->upload = 0;
	else
		throw ("Error: wrong syntax upload only takes on/off.");
}

void	Default_serv::set_upload(int data)
{
	this->upload = data;
}

void	Default_serv::set_autoindex(std::vector<std::string> data)
{
	if (!data[0].compare("on"))
		this->upload = 1;
	else if (!data[0].compare("off"))
		this->upload = 0;
	else
		throw ("Error: wrong syntax autoindex only takes on/off.");
}

void	Default_serv::set_autoindex(int data)
{
	this->autoindex = data;
}

void	Default_serv::set_allow_methods(std::vector<std::string> data)
{
	for (int	i = 0; i < static_cast<int>(data.size()); i++)
	{
		if (data[i].compare("GET") && data[i].compare("POST") && data[i].compare("DELETE"))
			throw ("Error: wrong syntax allow_methods only accept [GET - POST - DELETE].");
		this->allow_methods.push_back(data[i]);
	}
}

void	Default_serv::set_return(std::vector<std::string> data)
{
	check_status_code(data[0]);
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

//getters
std::vector<int> Default_serv::get_listen(void) const
{
	return (this->listen);
}

std::vector<std::string> Default_serv::get_index(void) const
{
	return (this->index);
}

std::vector<std::string> Default_serv::get_server_name(void) const
{
	return (this->server_name);
}

std::vector<std::pair<int, std::string> >	Default_serv::get_status_page(void) const
{
	return (this->status_page);
}

std::vector<std::pair<std::string, std::string> >	Default_serv::get_cgi_info(void) const
{
	return (this->cgi_info);
}

std::string Default_serv::get_host(void) const
{
	return (this->host);
}

std::string Default_serv::get_root(void) const
{
	return (this->root);
}

std::string Default_serv::get_client_max_body_size(void) const
{
	return (this->client_max_body_size);
}

int	Default_serv::get_upload(void) const
{
	return (this->upload);
}

int	Default_serv::get_autoindex(void) const
{
	return (this->autoindex);
}

std::vector<std::string> Default_serv::get_allow_methods(void) const
{
	return (this->allow_methods);
}

std::vector<std::pair<int, std::string> >	Default_serv::get_retur(void) const
{
	return (this->retur);
}

Default_serv::~Default_serv(void)
{
	//std::cout << "Default_serv default destructer." << std::endl;
}
