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
	this->servers = this->server;

}

std::vector<std::pair<t_tokens, std::string> >::iterator	Data::get_end_closing_braces(void)
{
	int	check;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	for (; this->begin->first != OPEN_BRACES && this->begin != this->tokens.end(); this->begin++)
		;
	for (this->begin++, iter = this->begin, check = 1; check && iter != this->tokens.end(); iter++)
	{
		if (iter->first == OPEN_BRACES)
			check++;
		else if (iter->first == CLOSE_BRACES)
			check--;
	}
	return (iter - 1);
}

Data::~Data()
{
	//std::cout << "Data Default destructer" << std::endl;
	//destructer
}
