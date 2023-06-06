#include "../include/Parsing.hpp"
//function tokens i need
//first keyword and then when i hold the string i will check if it's "server" or "location"
//space just put the token no string
//one function process open and close braces if we have '{{{' every single one in a token
//a function of semicolon if there's ';;;' it hold them all in one index

int	Parsing::iskeyword(int c)
{
	//if (c == ' ' || c == '{' || c == '}' || c == ';')
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
	//this->input.erase(this->start - this->input.begin(), this->iter - this->input.begin());
}

void	Parsing::space_token(void)
{
	while (*this->iter && *this->iter == ' ')
		this->iter++;
	//this->hold.first = SPACE;
	//here give it null
	//this->hold.second = ;
	//this->hold.second.clear();
	//this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::braces_token(t_tokens type)
{
	this->iter++;
	this->hold.first = type;
	//here give it null
	//this->hold.second = ;
	//this->hold.second.clear();
	if (type == OPEN_BRACES)
		this->hold.second = '{';
	else
		this->hold.second = '}';
	this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::semicolon_token(void)
{
	this->iter++;
	this->hold.first = SEMICOLON;
	//here give it null
	//this->hold.second.clear();
	this->hold.second = ';';
	this->tokens.push_back(this->hold);
	this->start = this->iter;
}

void	Parsing::tokenizer(void)
{
	//i will not include spaces
	//also the semicolon and braces take off the second string make it clear
	
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
