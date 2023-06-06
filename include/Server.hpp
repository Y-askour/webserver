#pragma once

#include "Default_serv.hpp"
#include "Location.hpp"

class Server : public Default_serv
{
	//here location
	std::map<std::string, Location> locations;
	public :
		Server();
		~Server();
};
