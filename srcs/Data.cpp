#include "../include/Data.hpp"

Data::Data()
{
	//std::cout << "Data Default constructer" << std::endl;
}

Data::Data(const Data & obj)
{
	//std::cout << "Data obj constructer" << std::endl;
	this->mime_types_parse = obj.mime_types_parse;
	this->servers = obj.servers;
	this->create_listen_sockets();

	//std::cout << "dddd" << std::endl;
}

std::vector<Server*> &Data::get_servers(void)
{
	return (this->servers);
}

void	Data::parse_file_and_syntax_error(void)
{
	//std::cout << this->servers[0]->l().begin()->second->get_root() << std::endl;
	std::cout << "d" << std::endl;
	std::cout << this->servers[0]->get_location().begin()->second->get_cgi_info()[0].first << std::endl;
}

Data::~Data()
{
	//std::cout << "Data Default destructer" << std::endl;
	//std::vector<Server*>::iterator itr;
	//for (itr = this->servers.begin(); itr != this->servers.end(); itr++)
	//	delete *itr;
}

void Data::create_listen_sockets()
{
	std::vector<int> ports;
	std::vector<Server *>::iterator it = this->servers.begin();
	while (it != this->servers.end())
	{
		std::vector<int> serverPort = (*it)->get_listen();
		std::vector<int>::iterator n = serverPort.begin();
		while (n != serverPort.end())
		{
			Connection socket(*(*it),*n);
			this->connections.push_back(socket);
			socket.get_fd();
			socket.get_something_from_server();
			n++;
		}
		it++;
	}
}
