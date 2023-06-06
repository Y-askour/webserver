#pragma once
#include "../include/Server.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

class	Parsing
{
	protected :
	typedef	enum tokens {
		SERVER,
		LOCATION,
		WORD,
		DIRECTIVE,
		OPEN_BRACES,
		CLOSE_BRACES,
		SEMICOLON,
	} t_tokens;

	//config file name
	std::string	config_file;
	//here where i put the file
	std::string	input;
	//this one hold tokens of the file
	std::vector<std::pair<t_tokens, std::string> > tokens;
	//this one have directive to check
	std::string	directive_id;
	//this one holds all the directive to check if directive true
	std::vector<std::pair<int, std::string> >	directive_name;
	//iterator string input
	std::string::iterator	start;
	std::string::iterator	iter;
	//iterator for vector tokens
	std::vector<std::pair<t_tokens, std::string> >::iterator	begin;
	std::vector<std::pair<t_tokens, std::string> >::iterator	end;
	//holds tokens of tokenizer to push in token
	std::pair<t_tokens, std::string> hold;

	//this vector hold all Servers and their location and i will init it to serves in data
	std::vector<Server> server;

	public :
	//here i should add copy constructer and shiit
		Parsing(void);
		Parsing(std::string file);
		void	turn_whitespaces_to_space(void);
		void	take_off_comments(void);
		void	parse_file(void);
		void	tokenizer(void);
		void	keyword_token(int chk);
		void	space_token(void);
		void	braces_token(t_tokens type);
		void	semicolon_token(void);
		int		iskeyword(int c);
		void	lexer(void);
		void	check_server_syntax(void);
		std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void);
		void	check_location_syntax();
		void	check_directive_syntax();
		void	check_allowed_directive(int check);
		int		find_directive(void);
		void	strtrim(void);
		void	turn_separator_to_space(void);

		//function to hold data in the server
		void	save_data_in_the_server(void);
		void	parse_server(void);
		~Parsing(void);
};
