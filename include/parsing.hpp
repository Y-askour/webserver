#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

class	parsing
{
	typedef	enum tokens {
		SERVER,
		LOCATION,
		WORD,
		//SPACE,
		DIRECTIVE,
		OPEN_BRACES,
		CLOSE_BRACES,
		SEMICOLON,
	} t_tokens;
	std::string	config_file;
	std::string	input;
	std::vector<std::pair<t_tokens, std::string> > tokens;

	//iterator
	std::string::iterator	start;
	std::string::iterator	iter;


	std::pair<t_tokens, std::string> hold;
	public :
	//here i should add copy constructer and shiit
		parsing(std::string file);
		void	turn_whitespaces_to_space(void);
		void	take_off_comments(void);
		void	parse_file(void);
		void	tokenizer(void);
		void	keyword_token(int chk);
		void	space_token(void);
		void	braces_token(t_tokens type);
		void	semicolon_token(void);
		int		iskeyword(int c);
		void	lexer();
		//void	_token(void);



		void	turn_separator_to_space(void);
		void	split(char sp);
		std::string	skip_spaces(int index, char sp);
		//void	ll(std::ifstream &in);
		void	strtrim(void);
		~parsing(void);
//method function
//function of parsing that contain all the function down blow
//function that reads the file
//function that splits the elment of the file and put it in a vector
//
};
