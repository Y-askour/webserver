/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Default_serv.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 02:32:10 by yaskour           #+#    #+#             */
/*   Updated: 2023/08/13 17:51:51 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Default_serv.hpp"

//this one for the server
Default_serv::Default_serv(void)
{
	this->listen.push_back(8080);
	this->server_name.push_back("localhost");

	//200
	this->status_page.push_back(std::make_pair(201, "status_page/201.html"));
	this->status_page.push_back(std::make_pair(204, "status_page/204.html"));

	// 300
	this->status_page.push_back(std::make_pair(301, "status_page/301.html"));

	// 400
	this->status_page.push_back(std::make_pair(400, "status_page/400.html"));
	this->status_page.push_back(std::make_pair(403, "status_page/403.html"));
	this->status_page.push_back(std::make_pair(404, "status_page/404.html"));
	this->status_page.push_back(std::make_pair(405, "status_page/405.html"));
	this->status_page.push_back(std::make_pair(409, "status_page/409.html"));
	this->status_page.push_back(std::make_pair(413, "status_page/413.html"));
	this->status_page.push_back(std::make_pair(414, "status_page/414.html"));

	// 500
	this->status_page.push_back(std::make_pair(500, "status_page/500.html"));
	this->status_page.push_back(std::make_pair(501, "status_page/501.html"));
	this->status_page.push_back(std::make_pair(505, "status_page/505.html"));

	this->host = "127.0.0.1";
	this->root = "var/www/";
	this->allow_methods.push_back("GET");
	this->client_max_body_size = "10000";
	this->upload = 0;
	this->autoindex = 0;
}

void Default_serv::get_non_init_data(const Default_serv &server) 
{
	if (!this->listen.size())
		this->listen = server.listen;

	if (!this->index.size())
		this->index = server.index;

	if (!this->server_name.size())
		this->server_name = server.server_name;

	if (!this->status_page.size())
		this->status_page = server.status_page;

	if (!this->cgi_info.size())
		this->cgi_info = server.cgi_info;

	if (!this->host.size())
		this->host = server.host;

	if (!this->root.size())
		this->root = server.root;

	if (!this->client_max_body_size.size())
		this->client_max_body_size = server.client_max_body_size;

	if (this->upload == -1)
		this->upload = server.upload;

	if (this->autoindex == -1)
		this->autoindex = server.autoindex;

	if (!this->allow_methods.size())
		this->allow_methods = server.allow_methods;

	if (!this->retur.size())
		this->retur = server.retur;
}

//this one for location
Default_serv::Default_serv(int) {
	this->upload = -1;
	this->autoindex = -1;
}

//parsing
void	Default_serv::take_off_default_setup(void)
{
	if (this->listen.size() > 1)
		this->listen.erase(this->listen.begin());
	if (this->server_name.size() > 1)
		this->server_name.erase(this->server_name.begin());
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
	if (data.compare("201") && data.compare("204") && data.compare("301") \
			&& data.compare("400") && data.compare("403") && data.compare("404") \
			&& data.compare("405") && data.compare("409") && data.compare("413") \
			&& data.compare("414") && data.compare("500") && data.compare("501") \
			&& data.compare("505"))
		throw ("Error: wrong status code, allow code [201 - 204 - 301 - 400 - 403 - 404 - 405 - 409 - 413 - 414 - 500 - 501 - 505].");
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
	if (atoi(data[0].c_str()) == 201)
		this->status_page[0].second = data[1];
	else if (atoi(data[0].c_str()) == 204)
		this->status_page[1].second = data[1];
	else if (atoi(data[0].c_str()) == 301)
		this->status_page[2].second = data[1];
	else if (atoi(data[0].c_str()) == 400)
		this->status_page[3].second = data[1];
	else if (atoi(data[0].c_str()) == 403)
		this->status_page[4].second = data[1];
	else if (atoi(data[0].c_str()) == 404)
		this->status_page[5].second = data[1];
	else if (atoi(data[0].c_str()) == 405)
		this->status_page[6].second = data[1];
	else if (atoi(data[0].c_str()) == 409)
		this->status_page[7].second = data[1];
	else if (atoi(data[0].c_str()) == 413)
		this->status_page[8].second = data[1];
	else if (atoi(data[0].c_str()) == 414)
		this->status_page[9].second = data[1];
	else if (atoi(data[0].c_str()) == 500)
		this->status_page[10].second = data[1];
	else if (atoi(data[0].c_str()) == 501)
		this->status_page[11].second = data[1];
	else if (atoi(data[0].c_str()) == 505)
		this->status_page[12].second = data[1];
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
}

void	Default_serv::set_root(std::vector<std::string> data)
{
	this->root = data[0];
}

void	Default_serv::set_client_max_body_size(std::vector<std::string> data)
{
	if (check_if_number(data[0]))
		throw ("Error: client_max_body_size only takes digit.");
	for (; data[0].length() && data[0][0] == '0';)
		data[0].erase(0, 1);
	if (data[0].empty())
		throw ("Error: client_max_body_size ranges from 1 to signed long long.");
	else {
		if (data[0].length() > 19)
			throw ("Error: client_max_body_size ranges from 1 to signed long long.");
		else if (data[0].length() == 19) {
			std::string hld = max_body;
			for (size_t i = 0; i != data[0].length(); i++) {
				if (data[0][i] > hld[i])
					throw ("Error: client_max_body_size ranges from 1 to signed long long.");
				else if (data[0][i] < hld[i])
					break ;
			}
		}
	}
	this->client_max_body_size = data[0];
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
		this->autoindex = 1;
	else if (!data[0].compare("off"))
		this->autoindex = 0;
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
}
