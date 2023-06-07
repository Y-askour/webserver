//#include "../include/Parsing.hpp"
#include "../include/Data.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	//Parsing	*data = new Data(av[1]);
	Data	data(av[1]);
	data.parse_file_and_syntax_error();

	//data.parse_file();
	//data.parse.parse_file();
	//Parsing	data(av[1]);
	//data.parse_file();
	return (0);
}
