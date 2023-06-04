#include "../include/parsing.hpp"

parsing::parsing(std::string file) : config_file(file)
{
	//constructer
}

//void	parsing::turn_separator_to_space(void)
//{
//	for (int i = 0; i < static_cast<int>(this->input.size()); i++)
//	{
//		for (int	j = 0; j < static_cast<int>(this->input[i].size()); j++)
//		{
//			if (this->input[i][j] == '\n')
//				std::cout << "wa3liha s3ra" << std::endl;
//			//if (isspace(this->input[i].c_str()[j]))
//			//	this->input[i][j] = ' ';
//		}
//		//exit(0);
//	}
//}

//std::string	parsing::skip_spaces(int index, char sp)
//{
//	std::string hold;
//
//	if (find(this->input[index].begin(), this->input[index].end(), sp) == this->input[index].end())
//	{
//		hold = this->input[index].substr(0, this->input[index].length());
//		this->input[index] = this->input[index].erase(0, this->input[index].length());
//		return (hold);
//		//return (this->input[0].substr(0, this->input[0].length()), \
//		//		this->input[0] = this->input[0].erase(0, this->input[0].length()));
//	}
//	//for (int	i = 0; i < static_cast<int>(this->input[0].size()); i++)
//	std::string::iterator	it = find(this->input[index].begin(), this->input[index].end(), sp);
//	hold = this->input[index].substr(0, it - this->input[index].begin());
//	//for (int i = it - this->input[0].begin(); this->input[0][]; i++)
//	//for (std::string::iterator i = it; *it != ' '; i++)
//	for (; *it == ' '; it++)
//		;
//	this->input[index] = this->input[index].erase(0, it - this->input[index].begin());
//	return (hold);
//}

//void	parsing::split(char sp)
//{
//	std::vector<std::string>	expressions;
//	std::string	hold;
//	//int	tmp;
//
//	//int	j;
//	//for (std::vector<std::string>::iterator it = this->input.begin(); \
//	//		it != this->input.end(); it++)
//	for (int i = 0; i < static_cast<int>(this->input.size()); i++)
//	{
//		//j = 0;
//		while (this->input[i].length())
//		{
//			expressions.push_back(this->skip_spaces(i, sp));
//			//std::cout << expressions[j] << std::endl;
//		}
//
//		//exit(0);
//		//tmp = 0;
//		//for (int	j = 0; this->input[i][j]; j++)
//		//std::string ll = "hey";
//		//std::cout << ll.find(' ') << std::endl;
//		//exit(0);
//		//std::string::iterator it = input[i].end();
//
//		//for (int	j = 0; find(input[i].begin(), input[i].end(), sp) != input[i].end() ; j++)
//		//while (find(input[i].begin(), input[i].end(), sp) != input[i].end())
//		//{
//		//	expressions.push_back(input[i].substr(0, input[i].find(sp)));
//		//	//expressions.push_back(input[i].substr(0, \
//		//	//	(find(input[i].begin(), input[i].end(), sp) != input[i].end()) ? input[i].find(sp): input[i].find(sp) + 1));
//		//	//input[i].erase(0, input[i].find(sp) + 1);
//		//	input[i].erase(0, \
//		//		(find(input[i].begin(), input[i].end(), sp) != input[i].end()) ? input[i].find(sp) + 1: input[i].find(sp));
//		//	std::cout << "|" << expressions[i] << "|" << std::endl;
//		//	std::cout << "|" << input[i] << "|" << std::endl;
//		//}
//		//std::cout << std::endl;
//		//for (int	k = 0; k < static_cast<int>(expressions.size()); k++)
//		//	std::cout << "|" << expressions[k] << "|"<< std::endl;
//		//exit(0);
//		//for (int	j = 0; j < static_cast<int>(this->input.size()); j++)
//		//{
//		//	if (this->input[i][j] == sp)
//		//	{
//		//		expressions.push_back(input[i].substr(0, j));
//		//		input[i].erase(0, j);
//		//		std::cout << "|" << expressions[i] << "|" << std::endl;
//		//		std::cout << "|" << input[i] << "|" << std::endl;
//		//		//exit(0);
//		//	}
//		//}
//		//exit(0);
//	}
//	this->input = expressions;
//}

//void	parsing::trim_string(void)
//{
//	int	start;
//	int	end;
//
//	for (std::vector<std::string>::iterator	it = this->input.begin(); it != this->input.end(); it++)
//	{
//		if (isspace((*it).c_str()[0]) || isspace((*it).c_str()[(*it).length()]))
//		{
//			for (start = 0; (*it)[start] == ' ' ; start++)
//				;
//			for (end = (*it).length() - 1; (*it)[end] == ' ' ; end--)
//				;
//			(*it) = (*it).substr(start, (end - start) + 1);
//		}
//	}
//}

void	parsing::take_off_comments(void)
{
	size_t	found;

	//for (int	i = 0; i < this->input.length(); i++)
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
	//std::string::iterator	tmp;
	//std::string::iterator	start = this->input.begin();
	//std::string::iterator	newline = std::find(this->input.begin(), this->input.end(), '\n');
	////mayebe here it will always find it 
	//while (newline != this->input.end())
	//{
	//	tmp = std::find(start, newline, '#');
	//	if (tmp != this->input.end())
	//	{
	//		this->input.erase(tmp - start, newline - tmp);
	//	}
	//	//newline++;
	//	//start = newline;
	//	start = std::find(this->input.begin(), this->input.end(), '\n') + 1;
	//	newline = std::find(start, this->input.end(), '\n');
	//	//std::cout << this->input;
	//	//exit(0);
	//}
}

void	parsing::strtrim(void)
{
	int	start, end;
	for (start = 0; isspace(this->input.c_str()[start]); start++)
		;
	for (end = this->input.length() - 1; isspace(this->input.c_str()[end]); end--)
		;
	//std::cout << start << std::endl;
	this->input = this->input.substr(start, (end + 1) - start);
}

void	parsing::turn_whitespaces_to_space(void)
{
	//for (size_t	found; \
	//	(found = this->input.find("\n\r\t\v\f")) <= static_cast<size_t>(this->input.length());)

	this->strtrim();

	//std::cout << "|" << this->input << "|";
	//exit(0);
	for (int	i = 0; this->input[i]; i++)
	{
		if (isspace(this->input.c_str()[i]))
			this->input[i] = ' ';
		//this->input.replace(found, 1, " ");
	}
}

void	parsing::parse_file(void)
{
	std::ifstream	in;

	in.open(this->config_file);
	if (!in.is_open())
	{
		std::cerr << "Error: file not exist" << std::endl;
		exit(1);
	}
	std::getline(in, this->input, '\0');
	//here always check if the file empty or full 
	//if (!this->input[0])
	if (this->input.empty())
	{
		std::cerr << "Error: empty config file" << std::endl;
		exit(1);
		//here error only
		//here provide a default config file
	}
	//first take off comments #
	this->take_off_comments();
	//second change all whitespaces to ' ' space
	//third trim the string
	this->turn_whitespaces_to_space();

	//fourth start tokenizing
	this->tokenizer();

	//fifth start lexer and grammer BNF
	this->lexer();

	//maybe here use try and catch
	



	//std::cout << this->input << std::endl;

	//here print the tokenizer vector
	for (int i = 0; i < static_cast<int>(this->tokens.size()); i++)
		std::cout << "|" << this->tokens[i].first << "|   |" << this->tokens[i].second << "|" << std::endl;



	//for (std::string input; std::getline(in, input, '\0'); )
	//	this->input.push_back(input);
	//this->turn_separator_to_space();
	//this->trim_string();
	//this->split(' ');
	//for (int i = 0; i < static_cast<int>(this->input.size()); i++)
	//	std::cout << "|" << this->input[i] << "|" << std::endl;
	//std::cout << std::endl;
}

parsing::~parsing(void)
{
	//destructer
}
