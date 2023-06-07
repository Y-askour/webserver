#include "../include/Parsing.hpp"

int	Parsing::iskeyword(int c)
{
	if (c == ' ' || c == '{' || c == '}' || c == ';' || c == 0)
		return (0);
	return (1);
}

void	Parsing::keyword_token(int chk)
{
	while (this->iskeyword(*this->iter))
		this->iter++;
	this->hold.first = WORD;
	if (chk)
		this->hold.first = DIRECTIVE;
	this->hold.second = this->input.substr(this->start - this->input.begin(), this->iter - this->start);
	if (!this->hold.second.compare("server"))
		this->hold.first = SERVER;
	else if (!this->hold.second.compare("location"))
		this->hold.first = LOCATION;
	this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::space_token(void)
{
	while (*this->iter && *this->iter == ' ')
		this->iter++;
	this->start = this->iter;
}

void	Parsing::braces_token(t_tokens type)
{
	this->iter++;
	this->hold.first = type;
	this->hold.second.clear();
	this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::semicolon_token(void)
{
	this->iter++;
	this->hold.first = SEMICOLON;
	this->hold.second.clear();
	this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::tokenizer(void)
{
	int	chk = 1;

	for (this->iter = this->input.begin(), this->start = this->iter; this->iter != this->input.end();)
	{
		switch (*this->iter)
		{
			case '{':
				this->braces_token(OPEN_BRACES);
				chk = 1;
				break ;
			case '}':
				this->braces_token(CLOSE_BRACES);
				chk = 1;
				break ;
			case ';':
				this->semicolon_token();
				chk = 1;
				break ;
			case ' ':
				this->space_token();
				break ;
			default:
				this->keyword_token(chk);
				chk = 0;
				break ;
		}
	}
}
