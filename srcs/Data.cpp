#include "../include/Data.hpp"

Data::Data() {
	//std::cout << "Data Default constructer" << std::endl;
}

Data::Data(const Data & obj)
{
	//std::cout << "Data obj constructer" << std::endl;
	*this = obj;
}

Data	& Data::operator=(const Data & obj)
{
	this->mime_types_parse = obj.mime_types_parse;
	for (int i = 0; i < static_cast<int>(obj.servers.size()); i++)
	{
		Server *server = new Server(*obj.servers[i]);
		this->servers.push_back(server);
	}
	return (*this);
}

std::vector<Server*> &Data::get_servers(void)
{
	return (this->servers);
}

Data::~Data()
{
	//std::cout << "Data Default destructer" << std::endl;
	std::vector<Server*>::iterator itr;
	for (itr = this->servers.begin(); itr != this->servers.end(); itr++)
		delete *itr;
}
