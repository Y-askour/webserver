//#include "../include/Parsing.hpp"
#include "../include/Data.hpp"
#include "../include/Parsing.hpp"

//void	print(std::vector<int>::iterator it)
void	print(int it)
{
	std::cout << it << std::endl;
}

int	main(int ac, char **av)
{
	//if the execubale have default use the default config file
	if (ac != 2)
	{
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	//Data	data(av[1]);
	//data.parse_file_and_syntax_error();

	//I WILL TAKE OFF MIME_TYPES IN DIRECTIVE
	//TOMMROW FIX IF LOCATION AND LISTEN REPEATED
	//AND CHECK IF EVERY VARIABLE HAVE DATA SO TO TAKE OFF THE DEFAULT SERVER
	//IF LOCATION EMPTY JUST JUST MAKE A COPY OF THE SERVER IN THE LOCATION USING COPY CONSTRUCTER
	//ALSO MAKE ERROR PAGES AND ADD THEM TO THE SERVER
	//maybe cgi i will not set it by defaul and check if repeated
	//also check client_max_body_size size
	//GAD SEGFAULT F STATUS_PAGE
	Parsing	*parse = new Parsing(av[1]);

	parse->parse_file();

	Data data(*parse);

	delete parse;

	//int i = 0;
	//while (i < static_cast<int>(data.servers.size()))
	//{
		std::cout << "--------------" << std::endl;
		//std::vector<int> test = 
		//std::cout <<  data.servers[i].get_listen() << std::endl;
		std::vector<int> zb= data.servers[0].get_listen();
		for (std::vector<int>::iterator it = zb.begin(); it != zb.end(); it++)
		{
			std::cout << *it << std::endl;
		}
		//std::for_each(data.servers[0].get_listen().begin(), data.servers[0].get_listen().end(), print);
		//i++;
	//}
	//data.print_config();


	//Parsing	*data = new Data(av[1]);

	//data.parse_file();
	//data.parse.parse_file();
	//Parsing	data(av[1]);
	//data.parse_file();
	return (0);
}
