#pragma once

#include "Default_serv.hpp"
#include "Location.hpp"

class Server : public Default_serv
{
	//here location
	public :
		std::map<std::string, Location&> locations;
		Server();
		Location	&locations_getter(void)
		{
			std::map<std::string, Location&>::iterator itr = this->locations.end();
			itr--;
			return (itr->second);
		}

		//void	check_if_location_repeated(void);

		int	get_location_size(void)
		{
			return (static_cast<int>(this->locations.size()));
		}

		//void	add_locations(std::pair<std::string, &Location> data)
		//void	add_locations(std::pair<std::string, Location> data)
		void	add_locations(std::pair<std::string, Location&> &data)
		{
			//(void)data;
			this->locations.insert(data);
		}
		~Server();
};
