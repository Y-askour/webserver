#include "../include/Data.hpp"
#include "../include/Parsing.hpp"
#include "../include/Connection.hpp"
#include <vector>

Parsing	&return_parsing_obj(int ac, char *av)
{
	if (ac == 2)
	{
		//Parsing	parse(av);
		Parsing	*parse = new Parsing(av);
		parse->parse_file();
		return (*parse);
	}
	Parsing	*parse = new Parsing;
	parse->make_default_server();
	return (*parse);
}

void	l()
{
	system("leaks webserv");
}

int	main(int ac, char **av)
{
	if (ac > 2)
	{
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	//in case of webserv empty kay5wer fe constructers and SEGV
	Data data(return_parsing_obj(ac, av[1]));
	return (0);
}


