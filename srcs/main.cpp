#include "../include/Data.hpp"
#include "../include/Parsing.hpp"

Parsing	&return_parsing_obj(int ac, char *av)
{
	if (ac == 2)
	{
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
	//if two servers don't have port to listen in change the port default port for every one of them
	//server { don't have port means have port 80 and the other one have port 80 errors}
	if (ac > 2)
	{
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	//in case of webserv empty kay5wer fe constructers and SEGV
	Parsing	*parse = &return_parsing_obj(ac, av[1]);
	//(void)pars;
	Data data(*parse);
	delete parse;
	//Data data(return_parsing_obj(ac, av[1]));
	atexit(l);
	return (0);
}
