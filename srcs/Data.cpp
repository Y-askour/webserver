#include "../include/Data.hpp"

Data::Data()
{
	//std::cout << "Data Default constructer" << std::endl;
	//constructer
}

Data::Data(const Data & obj)
{
	this->mime_types_parse = obj.mime_types_parse;
	this->servers = obj.servers;
}

Data::Data(std::string name)
{
	(void)name;
	//std::cout << "Data string constructer" << std::endl;
	//constructer
}

void	Data::parse_file_and_syntax_error(void)
{
	//this->parse_file();
	//this->servers = this->server;

}

Data::~Data()
{
	//std::cout << "Data Default destructer" << std::endl;
	//destructer
}
