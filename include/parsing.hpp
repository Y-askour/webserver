#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

class	parsing
{
	std::string	config_file;
	std::vector<std::string> input;
	public :
	//here i should add copy constructer and shiit
		parsing(std::string file);
		void	tokenizer(void);
		void	turn_separator_to_space(void);
		void	split(char sp);
		std::string	skip_spaces(int index, char sp);
		//void	ll(std::ifstream &in);
		void	trim_string(void);
		~parsing(void);
//method function
//function of parsing that contain all the function down blow
//function that reads the file
//function that splits the elment of the file and put it in a vector
//
};
