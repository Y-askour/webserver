#include "../include/Server.hpp"

Server::Server() : Default_serv()
{
}

//getter
std::map<std::string, Location*>	&Server::get_location(void)
{
	return (this->locations);
}

//setters
void	Server::set_locations(std::pair<std::string, Location*> data)
{
	this->locations.insert(data);
}

Server::~Server()
{
	std::map<std::string, Location*>::iterator itr;
	for (itr = this->locations.begin(); itr != this->locations.end(); itr++)
		delete itr->second;
}
