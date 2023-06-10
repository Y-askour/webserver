#include "../include/Server.hpp"

Server::Server() : Default_serv()
{
	//std::cout << "Server Default constructer" << std::endl;
	//constructer
}

//void	Server::check_if_location_repeated(void)
//{
//	std::map<std::string, Location>::iterator	itr;
//	for (itr = this->locations.begin(); itr != this->locations.end(); itr++)
//	{
//		//std::cout << itr->first << std::endl;
//		if (this->locations.count(itr->first) > 1)
//			throw ("Error: path of location is repeated.");
//	}
//	//(*locations.begin()).second.check_server_setup_duplicate();
//}

Server::~Server()
{
	//std::cout << "Server Default destructer" << std::endl;
	//destructer
}
