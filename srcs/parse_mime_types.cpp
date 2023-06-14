#include "../include/Parsing.hpp"

std::string	Parsing::get_mimetype_value(void)
{
	std::vector<std::pair<t_tokens, std::string> >::iterator	itr;
	std::string hold;

	for (itr = this->directive_itr + 1; itr->first != SEMICOLON; itr++)
	{
		if (itr->first == WORD)
			hold += itr->second + " ";
	}
	return (hold);
}

void	Parsing::lexer_of_mimetypes(void)
{
	this->begin = this->tokens.begin();

	if (this->begin->first == DIRECTIVE && !(this->begin++)->second.compare("types"))
	{
		if (this->begin->first == OPEN_BRACES && (this->tokens.end() - 1)->first == CLOSE_BRACES)
		{
			for (this->begin++; this->begin != this->tokens.end() - 1; this->begin++)
			{
				if (this->begin->first == DIRECTIVE)
				{
					this->directive_itr = this->begin;
					this->check_directive_syntax();
					this->mime_types_parse.insert(std::make_pair(this->directive_itr->second, get_mimetype_value()));
				}
				else
					throw ("Error : wrong syntax of the mime.types.");
			}
		}
		else
			throw ("Error : wrong syntax of the mime.types.");
	}
	else
		throw ("Error : wrong syntax of the mime.types.");
}

void	Parsing::parse_mime_types(void)
{
	try
	{
		std::fstream	in;
		in.open("config/mime.types");
		if (!in.is_open())
			throw ("Error: mime.types file not exist.");
		std::getline(in, this->input, '\0');
		if (this->input.empty())
			throw ("Error: empty config file.");
		this->turn_whitespaces_to_space();
		this->tokenizer();
		this->lexer_of_mimetypes();
		this->input.clear();
		this->tokens.erase(this->tokens.begin(), this->tokens.end());
		in.close();
	}
	catch (const char *error)
	{
		std::cerr << error << std::endl;
		exit(1);
	}
}
