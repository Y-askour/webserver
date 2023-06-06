/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_data_server.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkaddour <hkaddour@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 22:39:38 by hkaddour          #+#    #+#             */
/*   Updated: 2023/06/06 23:01:50 by hkaddour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parsing.hpp"

//parse location
//parse directive
//parse server
void	Parsing::parse_server(void)
{
	this->end = this->get_end_closing_braces();
	for (; this->begin != this->end; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
		{
			this->directive_id = this->begin->second;
			this->check_directive_syntax();
			this->check_allowed_directive(0);
		}
		else if (this->begin->first == LOCATION)
			this->check_location_syntax();
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
			//this->check_server_syntax();
			this->begin = this->end + 1;
		}
		//else
		//	throw ("Error: wrong syntax of the server.");
	}
}
