//#include "../include/Parsing.hpp"
#include "../include/Data.hpp"
#include "../include/Parsing.hpp"

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
	//data.print_config();


	//Parsing	*data = new Data(av[1]);

	//data.parse_file();
	//data.parse.parse_file();
	//Parsing	data(av[1]);
	//data.parse_file();
	return (0);
}
