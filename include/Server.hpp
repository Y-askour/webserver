#pragma once

#include "Default_serv.hpp"
#include "Location.hpp"

class Server : public Default_serv
{
	//here location
	std::map<std::string, Location> locations;
	public :
		Server();
		Location	locations_getter(void)
		{
			std::map<std::string, Location>::iterator itr = this->locations.end();
			itr--;
			return (itr->second);
			//return ((*(this->locations.end()--)).second);
		}
		void	add_locations(std::pair<std::string, Location> data)
		{
			this->locations.insert(data);
		}
		~Server();
};
