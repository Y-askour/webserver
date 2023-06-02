#include "../include/parsing.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	parsing	data(av[1]);
	data.tokenizer();
	return (0);
}
