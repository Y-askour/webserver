/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <yaskour@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:23:13 by yaskour           #+#    #+#             */
/*   Updated: 2023/05/28 20:35:00 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"

int main (int argc, char *argv[])
{	
	if (!(argc > 2))
	{
		std::cout << argv[argc - 1] << std::endl;
		parse_config_file();
		return 0;
	}
	return -1;
}
