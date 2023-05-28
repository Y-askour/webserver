/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <yaskour@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:33:10 by yaskour           #+#    #+#             */
/*   Updated: 2023/05/28 20:55:41 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

void parse_config_file(std::string file_name)
{
	std::fstream config_file;

	if (file_name.empty())
		throw std::string("no need for arg");

	config_file.open(file_name,std::fstream::in);
	if (!config_file.is_open())
		throw std::string(file_name + " is not found");
}
