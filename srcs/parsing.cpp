#include "../include/parsing.hpp"

parsing::parsing(std::string file) : config_file(file)
{
	//constructer
}

void	parsing::turn_separator_to_space(void)
{
	for (int i = 0; i < static_cast<int>(this->input.size()); i++)
	{
		for (int	j = 0; j < static_cast<int>(this->input[i].size()); j++)
		{
			if (isspace(this->input[i].c_str()[j]))
				this->input[i][j] = ' ';
				//this->input[i][j].erase();
		}
		//std::cout << this->input[i] << std::endl;
		//return ;
		//exit(0);
	}
}

std::string	parsing::skip_spaces(int index, char sp)
{
	std::string hold;

	if (find(this->input[index].begin(), this->input[index].end(), sp) == this->input[index].end())
	{
		hold = this->input[index].substr(0, this->input[index].length());
		this->input[index] = this->input[index].erase(0, this->input[index].length());
		return (hold);
		//return (this->input[0].substr(0, this->input[0].length()), \
		//		this->input[0] = this->input[0].erase(0, this->input[0].length()));
	}
	//for (int	i = 0; i < static_cast<int>(this->input[0].size()); i++)
	std::string::iterator	it = find(this->input[index].begin(), this->input[index].end(), sp);
	hold = this->input[index].substr(0, it - this->input[index].begin());
	//for (int i = it - this->input[0].begin(); this->input[0][]; i++)
	//for (std::string::iterator i = it; *it != ' '; i++)
	for (; *it == ' '; it++)
		;
	this->input[index] = this->input[index].erase(0, it - this->input[index].begin());
	return (hold);
}

void	parsing::split(char sp)
{
	std::vector<std::string>	expressions;
	std::string	hold;
	//int	tmp;

	//int	j;
	//for (std::vector<std::string>::iterator it = this->input.begin(); \
	//		it != this->input.end(); it++)
	for (int i = 0; i < static_cast<int>(this->input.size()); i++)
	{
		//j = 0;
		while (this->input[i].length())
		{
			expressions.push_back(this->skip_spaces(i, sp));
			//std::cout << expressions[j] << std::endl;
		}

		//exit(0);
		//tmp = 0;
		//for (int	j = 0; this->input[i][j]; j++)
		//std::string ll = "hey";
		//std::cout << ll.find(' ') << std::endl;
		//exit(0);
		//std::string::iterator it = input[i].end();

		//for (int	j = 0; find(input[i].begin(), input[i].end(), sp) != input[i].end() ; j++)
		//while (find(input[i].begin(), input[i].end(), sp) != input[i].end())
		//{
		//	expressions.push_back(input[i].substr(0, input[i].find(sp)));
		//	//expressions.push_back(input[i].substr(0, \
		//	//	(find(input[i].begin(), input[i].end(), sp) != input[i].end()) ? input[i].find(sp): input[i].find(sp) + 1));
		//	//input[i].erase(0, input[i].find(sp) + 1);
		//	input[i].erase(0, \
		//		(find(input[i].begin(), input[i].end(), sp) != input[i].end()) ? input[i].find(sp) + 1: input[i].find(sp));
		//	std::cout << "|" << expressions[i] << "|" << std::endl;
		//	std::cout << "|" << input[i] << "|" << std::endl;
		//}
		//std::cout << std::endl;
		//for (int	k = 0; k < static_cast<int>(expressions.size()); k++)
		//	std::cout << "|" << expressions[k] << "|"<< std::endl;
		//exit(0);
		//for (int	j = 0; j < static_cast<int>(this->input.size()); j++)
		//{
		//	if (this->input[i][j] == sp)
		//	{
		//		expressions.push_back(input[i].substr(0, j));
		//		input[i].erase(0, j);
		//		std::cout << "|" << expressions[i] << "|" << std::endl;
		//		std::cout << "|" << input[i] << "|" << std::endl;
		//		//exit(0);
		//	}
		//}
		//exit(0);
	}
	this->input = expressions;
}

void	parsing::trim_string(void)
{
	int	start;
	int	end;

	for (std::vector<std::string>::iterator	it = this->input.begin(); it != this->input.end(); it++)
	{
		if (isspace((*it).c_str()[0]) || isspace((*it).c_str()[(*it).length()]))
		{
			for (start = 0; (*it)[start] == ' ' ; start++)
				;
			for (end = (*it).length() - 1; (*it)[end] == ' ' ; end--)
				;
			(*it) = (*it).substr(start, (end - start) + 1);
		}
	}
}

void	parsing::tokenizer(void)
{
	std::ifstream	in;

	in.open(this->config_file);
	if (!in.is_open())
	{
		std::cerr << "Error: file not exist" << std::endl;
		exit(1);
	}
	for (std::string input; std::getline(in, input); )
		this->input.push_back(input);
	this->turn_separator_to_space();
	this->trim_string();
	this->split(' ');
	for (int i = 0; i < static_cast<int>(this->input.size()); i++)
		std::cout << "|" << this->input[i] << "|" << std::endl;
}

parsing::~parsing(void)
{
	//destructer
}
