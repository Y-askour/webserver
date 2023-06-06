#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

class	Parsing
{
	//#define	directive "listen host root index mime_types server_name status_page allow_methods client_max_body_size autoindex return cgi upload";
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

	//this one have directive to check
	std::string	directive_id;
	std::vector<std::pair<int, std::string> >	directive_name;

	//iterator string input
	std::string::iterator	start;
	std::string::iterator	iter;

	//iterator for vector tokens
	std::vector<std::pair<t_tokens, std::string> >::iterator	begin;
	std::vector<std::pair<t_tokens, std::string> >::iterator	end;


	std::pair<t_tokens, std::string> hold;
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
		//std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_directive(void);
		



		void	turn_separator_to_space(void);
		void	split(char sp);
		std::string	skip_spaces(int index, char sp);
		//void	ll(std::ifstream &in);
		void	strtrim(void);
		~Parsing(void);
//method function
//function of parsing that contain all the function down blow
//function that reads the file
//function that splits the elment of the file and put it in a vector
//
};
