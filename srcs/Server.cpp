#include "../include/Server.hpp"

Server::Server() : Default_serv()
{
	//std::cout << "server constructer" << std::endl;
}

Server::Server(Server & obj) : Default_serv(obj)
{
	*this = obj;
}

//Server & Server::operator=(const Server & obj)
Server & Server::operator=(Server & obj)
{
	std::map<std::string, Location*>::iterator	itr;

	for (itr = obj.get_location().begin(); itr != obj.locations.end(); itr++)
	{
		Location *location = new Location(*itr->second);
		this->set_locations(std::make_pair(itr->first, location));
	}
	return (*this);
}

//getter
std::map<std::string, Location*>	& Server::get_location(void)
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
	//std::cout << "Server Default destructer" << std::endl;
	std::map<std::string, Location*>::iterator itr;
	for (itr = this->locations.begin(); itr != this->locations.end(); itr++)
		delete itr->second;
	//here delete location
}
