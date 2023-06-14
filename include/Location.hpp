#pragma once

#include "Default_serv.hpp"
#include <iostream>
#include <vector>

class	Location : public Default_serv
{
	public :
		Location(void);
		Location(Default_serv const & obj);
		Location & operator=(Default_serv const & obj);
		~Location(void);
};
