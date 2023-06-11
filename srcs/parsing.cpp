#include "../include/Parsing.hpp"

Parsing::Parsing(void)
{
	//std::cout << "Parsing Default constructer" << std::endl;
}

//std::string	Parsing::get_mimetype_value(void)
//{
//	std::vector<std::pair<t_tokens, std::string> >::iterator	itr;
//	std::string hold;
//
//	for (itr = this->directive_itr + 1; itr->first != SEMICOLON; itr++)
//	{
//		if (itr->first == WORD)
//			hold += itr->second + " ";
//	}
//	return (hold);
//}
//
//void	Parsing::lexer_of_mimetypes(void)
//{
//	this->begin = this->tokens.begin();
//
//	if (this->begin->first == DIRECTIVE && !(this->begin++)->second.compare("types"))
//	{
//		if (this->begin->first == OPEN_BRACES && (this->tokens.end() - 1)->first == CLOSE_BRACES)
//		{
//			for (this->begin++; this->begin != this->tokens.end() - 1; this->begin++)
//			{
//				if (this->begin->first == DIRECTIVE)
//				{
//					this->directive_itr = this->begin;
//					this->check_directive_syntax();
//					this->mime_types_parse.insert(std::make_pair(this->directive_itr->second, get_mimetype_value()));
//				}
//				else
//					throw ("Error : wrong syntax of the mime.types.");
//			}
//		}
//		else
//			throw ("Error : wrong syntax of the mime.types.");
//	}
//	else
//		throw ("Error : wrong syntax of the mime.types.");
//}

Parsing::Parsing(std::string file) : config_file(file)
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

	//here gad blan dial meme_types.
	this->parse_mime_types();
	//try
	//{
	//	std::fstream	in;
	//	//here catch the exception
	//	in.open("config/mime.types");
	//	if (!in.is_open())
	//		throw ("Error: mime.types file not exist.");
	//	std::getline(in, this->input, '\0');
	//	if (this->input.empty())
	//		throw ("Error: empty config file.");
	//	this->turn_whitespaces_to_space();
	//	this->tokenizer();
	//	this->lexer_of_mimetypes();
	//	//this in the end.
	//	this->input.clear();
	//	//here free the tokens
	//	//this->tokens.erase(this->tokens.begin(), this->tokens.end());
	//	this->tokens.erase(this->tokens.begin(), this->tokens.end());
	//	//exit(0);
	//}
	//catch (const char *error)
	//{
	//	std::cerr << error << std::endl;
	//	exit(1);
	//}
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
	for (int	i = 0; i < static_cast<int>(this->servers.size()); i++)
	{
		this->servers[i].take_off_default_setup();
		this->servers[i].check_server_setup_duplicate();
		//this->servers[i].check_server_setup_duplicate();
		//this->servers[i].check_if_location_repeated();
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
		//while (1);
		//here check the final error like if listen have same port and location path and some like that
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
	//destructer
}
