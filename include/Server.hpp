#pragma once

#include "Default_serv.hpp"
#include "Location.hpp"

class Server : public Default_serv
{
	//here location
	std::map<std::string, Location*> locations;

	public :
		Server();
		//Server(const Server & obj);
		Server(Server & obj);
		//Server & operator=(const Server & obj);
		Server & operator=(Server & obj);

		//getters
		std::map<std::string, Location*>	& get_location(void);

		//setters
		void	set_locations(std::pair<std::string, Location*> data);

		~Server();
};
