#pragma once

#include <iostream>
#include <vector>
#include "Server.hpp"
#include "Parsing.hpp"

class Data
{
	std::vector<Server> servers;
	Parsing	parse("kj");
	public :
		Data();
		~Data();
};
