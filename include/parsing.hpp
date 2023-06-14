#pragma once
#include "Data.hpp"

class	Parsing : public Data
{
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
	//here where i put the file when i read it
	std::string	input;
	//this one hold tokens of the file
	std::vector<std::pair<t_tokens, std::string> > tokens;
	//this one have begin directive to check
	std::vector<std::pair<t_tokens, std::string> >::iterator	directive_itr;
	//this one holds all the directive to check if directive true
	std::vector<std::pair<int[3], std::string> >	directive_name; //int index 1 for server and 2 for location
	//iterator string input
	std::string::iterator	start;
	std::string::iterator	iter;
	//iterator for vector tokens
	std::vector<std::pair<t_tokens, std::string> >::iterator	begin;
	std::vector<std::pair<t_tokens, std::string> >::iterator	end;
	//holds tokens of tokenizer to push in token
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
		std::vector<std::pair<Parsing::t_tokens, std::string> >::iterator	get_end_closing_braces(void);
		std::vector<std::pair<Parsing::t_tokens, std::string> >::iterator	get_end_closing_braces(int);
		void	check_location_syntax();
		void	check_directive_syntax();
		void	check_allowed_directive_and_repetitive(int check);
		std::vector<std::pair<int[3], std::string> >::iterator	find_directive(void);
		void	strtrim(void);
		void	turn_sparse_mime_typeseparator_to_space(void);
		void	check_directive_value_length(void);

		//function to hold data in the server
		void	save_data_in_the_server(void);
		void	parse_server(void);
		void	parse_location(void);
		void	parse_directive(int check);
		void	check_wish_directive(int check, std::vector<std::string> data);

		//Parsing of mime_types
		void	lexer_of_mimetypes(void);
		std::string	get_mimetype_value(void);
		void	parse_mime_types(void);

		//last steps check duplicate element and take off default setup
		void	last_check_servers(void);
		//this is for default server building
		void	make_default_server(void);
		~Parsing(void);
};
