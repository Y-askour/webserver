#include "../include/parsing.hpp"

std::vector<std::pair<parsing::t_tokens, std::string> >::iterator	parsing::get_end_closing_braces(void)
{
	int	check;
	std::vector<std::pair<parsing::t_tokens, std::string> >::iterator	iter;

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

int	parsing::find_directive(void)
{
	for (int	i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		if (!this->directive_name[i].second.compare(this->directive_id))
			return (this->directive_name[i].first);
	}
	return (-1);
}

void	parsing::check_allowed_directive(int check)
{
	int	find = this->find_directive();
	if (find < 0)
	{
		//std::cout << this->directive_id << std::endl;
		throw ("Error: use of unexisting directive.");
	}
	if (!check)
	{
		if (find == 1)
			throw ("Error: misplaced directive place only use in location.");
	}
	else
	{
		if (find == 0)
			throw ("Error: misplaced directive place only use in server.");
	}
}

void	parsing::check_directive_syntax(void)
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

void	parsing::check_location_syntax(void)
{
	std::vector<std::pair<parsing::t_tokens, std::string> >::iterator	iter;

	if ((this->begin + 2) == this->tokens.end())
		throw ("Error: wrong syntax of location, usage location path {...}.");
	if ((++this->begin)->first != WORD || (++this->begin)->first != OPEN_BRACES)
		throw ("Error: wrong syntax of location, usage location path {...}.");
	this->begin++;
	iter = this->get_end_closing_braces();
	for (; this->begin != iter; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->directive_id = this->begin->second;
			this->check_directive_syntax();
			this->check_allowed_directive(1);
		}
		else
			throw ("Error: wrong syntax.");
	}
}

void	parsing::check_server_syntax(void)
{
	if ((++this->begin) == this->tokens.end() || (this->begin++)->first != OPEN_BRACES)
		throw ("Error: wrong syntax of the server.");
	this->end = this->get_end_closing_braces();
	for (; this->begin != this->end; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->directive_id = this->begin->second;
			this->check_directive_syntax();
			this->check_allowed_directive(0);
		}
		else if (this->begin->first == LOCATION)
			this->check_location_syntax();
		else
			throw ("Error: wrong syntax.");
	}
}

void	parsing::lexer(void)
{
	for (this->begin = this->tokens.begin(); this->begin != this->tokens.end(); )
	{
		if (this->begin->first == SERVER)
		{
			this->check_server_syntax();
			this->begin = this->end + 1;
		}
		else
			throw ("Error: wrong syntax of the server.");
	}
}
