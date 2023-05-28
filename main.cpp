/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <yaskour@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:23:13 by yaskour           #+#    #+#             */
/*   Updated: 2023/05/28 20:52:30 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"

int main (int argc, char *argv[])
{	
	if (!(argc > 2))
	{
		try 
		{
			if (argc == 2)
				parse_config_file(argv[1]);
			else
				parse_config_file(NULL);
			return 0;
		}
		catch (std::string &e)
		{
			std::cerr << e << std::endl;
		}
	}
	return -1;
}
