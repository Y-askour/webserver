#include "../include/Parsing.hpp"

std::vector<std::pair<Parsing::t_tokens, std::string> >::iterator	Parsing::get_end_closing_braces(void)
{
	int	check;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	for (iter = this->begin, check = 1; check && iter != this->tokens.end(); iter++)
	{
		if (iter->first == OPEN_BRACES)
			check++;
		else if (iter->first == CLOSE_BRACES)
			check--;
	}
	if (check)
		throw ("Error: missing close braces of the server.");
	return (iter - 1);
}

std::vector<std::pair<int[3], std::string> >::iterator	Parsing::find_directive(void)
{
	std::vector<std::pair<int[3], std::string> >::iterator	itr;

	for (itr = this->directive_name.begin(); itr != this->directive_name.end(); itr++)
	{
		if (!itr->second.compare(this->directive_itr->second))
			return (itr);
	}
	return (this->directive_name.end());
}

void	Parsing::check_allowed_directive_and_repetitive(int check)
{
	std::vector<std::pair<int[3], std::string> >::iterator	iter;

	iter = this->find_directive();
	if (iter == this->directive_name.end())
		throw ("Error: use of unexisting directive.");
	//server
	if (!check)
	{
		if (iter->first[0] == 1)
			throw ("Error: misplaced directive place only use in location.");
		if ((!iter->second.compare("host") || !iter->second.compare("root") \
				|| !iter->second.compare("upload") || !iter->second.compare("autoindex") \
				|| !iter->second.compare("allow_methods") || !iter->second.compare("client_max_body_size")))
		{
			iter->first[1]++;
			if (iter->first[1] > 1)
				throw ("Error: wrong syntax repeated directive in server.");
		}
	}
	//location
	else
	{
		if (iter->first[0] == 0)
			throw ("Error: misplaced directive place only use in server.");
		if ((!iter->second.compare("host") || !iter->second.compare("root") \
				|| !iter->second.compare("upload") || !iter->second.compare("autoindex") \
				|| !iter->second.compare("allow_methods") || !iter->second.compare("client_max_body_size")))
		{
			iter->first[2]++;
			if (iter->first[2] > 1)
				throw ("Error: wrong syntax repeated directive in location.");
		}
	}
}

void	Parsing::check_directive_syntax(void)
{
	int	count;

	for (count = 0, this->begin++; \
			this->begin != this->tokens.end() && this->begin->first != SEMICOLON; this->begin++, count++)
	{
		if (this->begin->first != WORD)
			throw ("Error: wrong syntax of directive, usage directive word;.");
	}
	if (this->begin == this->tokens.end() || !count)
		throw ("Error: wrong syntax of directive, usage directive word;.");
}

void	Parsing::check_directive_value_length()
{
	int	count;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;
	for (iter = this->directive_itr, count = 0; iter->first != SEMICOLON; iter++)
	{
		if (iter->first == WORD)
			count++;
	}
	if ((!this->directive_itr->second.compare("listen") || !this->directive_itr->second.compare("host") \
			|| !this->directive_itr->second.compare("root") || !this->directive_itr->second.compare("autoindex") \
			|| !this->directive_itr->second.compare("upload") \
			|| !this->directive_itr->second.compare("client_max_body_size")) && (count != 1))
		throw ("Error: wrong syntax directive take one parameter.");
	if ((!this->directive_itr->second.compare("status_page") || !this->directive_itr->second.compare("return") \
				|| !this->directive_itr->second.compare("cgi_info")) && (count != 2))
		throw ("Error: wrong syntax directive take two parameter.");
}

void	Parsing::check_location_syntax(void)
{
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	if ((this->begin + 2) == this->tokens.end())
		throw ("Error: wrong syntax of location, usage location path {...}.");
	if ((++this->begin)->first != WORD || (++this->begin)->first != OPEN_BRACES)
		throw ("Error: wrong syntax of location, usage location path {...}.");
	this->begin++;
	iter = this->Parsing::get_end_closing_braces();
	for (; this->begin != iter; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->directive_itr = this->begin;
			this->check_directive_syntax();
			this->check_allowed_directive_and_repetitive(1);
			this->check_directive_value_length();
		}
		else
			throw ("Error: wrong syntax.");
	}
	for (int i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		this->directive_name[i].first[1] = 0;
		this->directive_name[i].first[2] = 0;
	}
}

void	Parsing::check_server_syntax(void)
{
	if ((++this->begin) == this->tokens.end() || (this->begin++)->first != OPEN_BRACES)
		throw ("Error: wrong syntax of the server.");
	this->end = this->Parsing::get_end_closing_braces();
	for (; this->begin != this->end; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->directive_itr = this->begin;
			this->check_directive_syntax();
			this->check_allowed_directive_and_repetitive(0);
			this->check_directive_value_length();
		}
		else if (this->begin->first == LOCATION)
			this->check_location_syntax();
		else
			throw ("Error: wrong syntax.");
	}
}

void	Parsing::lexer(void)
{
	for (this->begin = this->tokens.begin(); this->begin != this->tokens.end(); )
	{
		if (this->begin->first == SERVER)
		{
			this->check_server_syntax();
			for (int i = 0; i < static_cast<int>(this->directive_name.size()); i++)
			{
				this->directive_name[i].first[1] = 0;
				this->directive_name[i].first[2] = 0;
			}
			this->begin = this->end + 1;
		}
		else
			throw ("Error: wrong syntax of the server.");
	}
}
