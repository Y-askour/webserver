/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_data_server.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkaddour <hkaddour@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:39:38 by hkaddour          #+#    #+#             */
/*   Updated: 2023/06/07 18:44:36 by hkaddour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//in lexer check if directive duplicate
//and in here when to save data check with std::count if parameter are repeated
//also i will check location /path when i enter it in map
//******NOW I WILL HANDLE IF WORD ARE REPETED FOR LISTEN AND LOCATION also check int value like listen 222
#include "../include/Parsing.hpp"


//parse location
//parse directive

//std::vector<std::pair<t_tokens, std::string> >::iterator	get_end_closing_braces(void)
//{
//	std::cout << "hey " << std::endl;
//	exit(0);
//}

void	Parsing::check_wish_directive(std::vector<std::string> data)
{
	typedef void(Default_serv::*func)(std::vector<std::string>);
	func serv_func[] = {&Default_serv::add_listen, &Default_serv::add_host, \
		&Default_serv::add_mime_types, &Default_serv::add_server_name, &Default_serv::add_status_page, \
			&Default_serv::add_return, &Default_serv::add_root, &Default_serv::add_index, \
			&Default_serv::add_allow_methods, &Default_serv::add_client_max_body_size, \
			&Default_serv::add_auto_index, &Default_serv::add_cgi_info, &Default_serv::add_upload};
	for (int	i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		if (!this->directive_itr->second.compare(this->directive_name[i].second))
			((*this->server.end()).*serv_func[i])(std::vector<std::string>(data));
			//((*this->server.end()).*serv_func[i])(std::vector<std::string>());
	}
}

void	Parsing::parse_directive(void)
{
	std::vector<std::string>	data;
	this->directive_itr = this->begin;
	for (this->begin++; this->begin != this->tokens.end() && this->begin->first != SEMICOLON; this->begin++)
		data.push_back(this->begin->second);
	this->check_wish_directive(data);
}

//parse server
void	Parsing::parse_server(void)
{
	Server	serv;

	this->server.push_back(serv);
	this->end = this->get_end_closing_braces();
	for (; this->begin != this->end; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->parse_directive();
			//this->check_directive_syntax();
		}
		//else if (this->begin->first == LOCATION)
		//	this->parse_location();
			//this->check_location_syntax();
	}
}

//tomorrow continue working
void	Parsing::save_data_in_the_server(void)
{
	//Server server;
	
	for (this->begin = this->tokens.begin(); this->begin != this->tokens.end(); )
	{
		if (this->begin->first == SERVER)
		{
			this->parse_server();
			this->begin = this->end + 1;
		}
	}
}
