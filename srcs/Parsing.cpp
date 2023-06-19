#include "../include/Parsing.hpp"

Parsing::Parsing(void) : Data()
{
	//std::cout << "Parsing Default constructer" << std::endl;
}

Parsing::Parsing(std::string file) : Data(), config_file(file)
{
	//std::cout << "Parsing string constructer" << std::endl;
	std::pair<int[3], std::string> data;
	std::string	directives = "listen host server_name status_page return root index allow_methods client_max_body_size autoindex cgi_info upload ";

	for (int	i = 0, key = 0; !directives.empty(); i++)
	{
		if (i == 4)
			key = 1;
		else if (i > 4)
			key = 2;
		data.first[0] = key;
		data.first[1] = 0;
		data.first[2] = 0;
		data.second = directives.substr(0, directives.find(' '));
		directives.erase(0, directives.find(' ') + 1);
		this->directive_name.push_back(data);
	}
	this->parse_mime_types();
}

void	Parsing::make_default_server(void)
{
	this->parse_mime_types();
	Server	*serv = new Server();
	this->servers.push_back(serv);
}

void	Parsing::take_off_comments(void)
{
	size_t	found;

	while (1)
	{
		found = this->input.find('#');
		if (found <= static_cast<size_t>(this->input.length()))
		{
			this->input.erase(found, \
				std::distance(this->input.begin(), \
					std::find(this->input.begin() + found, this->input.end(), '\n')) - found);
		}
		else
			break ;
	}
}

void	Parsing::strtrim(void)
{
	int	start, end;
	for (start = 0; isspace(this->input.c_str()[start]); start++)
		;
	for (end = this->input.length() - 1; isspace(this->input.c_str()[end]); end--)
		;
	this->input = this->input.substr(start, (end + 1) - start);
}

void	Parsing::turn_whitespaces_to_space(void)
{
	this->strtrim();
	for (int	i = 0; this->input[i]; i++)
	{
		if (isspace(this->input.c_str()[i]))
			this->input[i] = ' ';
	}
}

void	Parsing::last_check_servers(void)
{
	//this vector have all servers port to check if any server have the same port that listen in
	std::vector<int> port_checking;

	for (int	i = 0; i < static_cast<int>(this->servers.size()); i++)
	{
		this->servers[i]->take_off_default_setup();
		this->servers[i]->check_server_setup_duplicate(port_checking);
	}
	std::vector<int>::iterator itr;
	for (itr = port_checking.begin(); itr != port_checking.end(); itr++)
	{
		if (std::count(port_checking.begin(), port_checking.end(), *itr) > 1)
			throw ("Error: servers have duplicate ports.");
	}
}

void	Parsing::parse_file(void)
{
	try
	{
		std::ifstream	in;
	
		in.open(this->config_file);
		if (!in.is_open())
			throw ("Error: file not exist");
		std::getline(in, this->input, '\0');
		if (this->input.empty())
			throw ("Error: empty config file.");
		this->take_off_comments();
		this->turn_whitespaces_to_space();
		this->tokenizer();
		this->lexer();
		this->save_data_in_the_server();
		this->last_check_servers();
		in.close();
	}
	catch (const char *error)
	{
		std::cerr << error << std::endl;
		exit(1);
	}
}

Parsing::~Parsing(void)
{
	//std::cout << "Parsing Default destructer" << std::endl;
	//std::vector<Server*>::iterator itr;
	//for (itr = this->servers.begin(); itr != this->servers.end(); itr++)
	//	delete *itr;
	//destructer
}
