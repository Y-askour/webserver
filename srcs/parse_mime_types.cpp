#include "../include/Parsing.hpp"

void	Parsing::parse_mime_types(void)
{
	std::string hold;
	for (int i = 0; i < static_cast<int>(this->server.size()); i++)
	{
		//here i should check if mime_type of the first server is the same as the next one to not parse again
		//just give it the first one

		hold = this->server[i].get_mime_types();
	}
}
