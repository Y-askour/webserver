/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_data_server.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkaddour <hkaddour@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:39:38 by hkaddour          #+#    #+#             */
/*   Updated: 2023/06/08 10:49:37 by hkaddour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//in lexer check if directive duplicate
//and in here when to save data check with std::count if parameter are repeated
//also i will check location /path when i enter it in map
//******NOW I WILL HANDLE IF WORD ARE REPETED FOR LISTEN AND LOCATION also check int value like listen 222
#include "../include/Parsing.hpp"

void	Parsing::check_wish_directive(int check, std::vector<std::string> data)
{
	typedef void(Default_serv::*func)(std::vector<std::string>);
	func serv_func[] = {&Default_serv::add_listen, &Default_serv::add_host, \
		&Default_serv::add_mime_types, &Default_serv::add_server_name, &Default_serv::add_status_page, \
			&Default_serv::add_return, &Default_serv::add_root, &Default_serv::add_index, \
			&Default_serv::add_allow_methods, &Default_serv::add_client_max_body_size, \
			&Default_serv::add_autoindex, &Default_serv::add_cgi_info, &Default_serv::add_upload};
	for (int	i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		if (!this->directive_itr->second.compare(this->directive_name[i].second))
		{
			if (!check)
				((*(this->server.end() - 1)).*serv_func[i])(data);
			else
				((*(this->server.end() - 1)).locations_getter().*serv_func[i])(data);
			return ;
		}
	}
}

void	Parsing::parse_directive(int check)
{
	std::vector<std::string>	data;
	this->directive_itr = this->begin;
	for (this->begin++; this->begin != this->tokens.end() && this->begin->first != SEMICOLON; this->begin++)
		data.push_back(this->begin->second);
	this->check_wish_directive(check, data);
}

void	Parsing::parse_location(void)
{
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;
	Location	location;
	(*(this->server.end() - 1)).add_locations(std::make_pair((this->begin + 1)->second, location));
	iter = this->get_end_closing_braces();
	for (; this->begin != iter; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
			this->parse_directive(1);
	}
}

void	Parsing::parse_server(void)
{
	Server	serv;

	this->server.push_back(serv);
	this->end = this->get_end_closing_braces();
	for (; this->begin != this->end; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
			this->parse_directive(0);
		else if (this->begin->first == LOCATION)
			this->parse_location();
	}
}

void	Parsing::save_data_in_the_server(void)
{
	for (this->begin = this->tokens.begin(); this->begin != this->tokens.end(); )
	{
		if (this->begin->first == SERVER)
		{
			this->parse_server();
			this->begin = this->end + 1;
		}
	}
}