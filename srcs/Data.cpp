#include "../include/Data.hpp"

Data::Data()
{
	//std::cout << "Data Default constructer" << std::endl;
	//constructer
}

Data::Data(std::string name) : Parsing(name)
{
	//std::cout << "Data string constructer" << std::endl;
	//constructer
}

void	Data::parse_file_and_syntax_error(void)
{
	this->parse_file();
	//this->servers = this->server;
}

std::vector<std::pair<t_tokens, std::string> >::iterator	Data::get_end_closing_braces(void)
{
	//int	check;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	for (iter = this->begin; iter != this->tokens.end(); iter++)
		;
	//{
	//	if (iter->first == OPEN_BRACES)
	//		check++;
	//	else if (iter->first == CLOSE_BRACES)
	//		check--;
	//}
	//if (check)
	//	throw ("Error: missing close braces of the server.");
	return (iter - 1);
}

Data::~Data()
{
	//std::cout << "Data Default destructer" << std::endl;
	//destructer
}
