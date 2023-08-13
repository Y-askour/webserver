/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Default_serv.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 02:32:28 by yaskour           #+#    #+#             */
/*   Updated: 2023/08/13 17:43:03 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#define max_body "9223372036854775807"

class Default_serv
{
	//they can be repeated
	std::vector<int>	listen;
	std::vector<std::string>	index;
	std::vector<std::string>	server_name;
	std::vector<std::pair<int, std::string> > status_page;
	std::vector<std::pair<std::string, std::string> > cgi_info;
	//those ones never repeated
	std::string	host;
	std::string root;
	std::string	client_max_body_size;
	int	upload;
	int	autoindex;
	std::vector<std::string>	allow_methods;
	//this one for location
	std::vector<std::pair<int, std::string> > retur;

	public :
		Default_serv(void);
		Default_serv(int);
		void get_non_init_data(const Default_serv&server);
		//setters
		void	set_listen(std::vector<std::string> data);
		void	set_index(std::vector<std::string> data);
		void	set_server_name(std::vector<std::string> data);
		void	set_status_page(std::vector<std::string> data);
		void	set_cgi_info(std::vector<std::string> data);
		void	set_host(std::vector<std::string> data);
		void	set_root(std::vector<std::string> data);
		void	set_client_max_body_size(std::vector<std::string> data);
		void	set_upload(std::vector<std::string> data);
		void	set_autoindex(std::vector<std::string> data);
		void	set_upload(int data);
		void	set_autoindex(int data);
		void	set_allow_methods(std::vector<std::string> data);
		void	set_return(std::vector<std::string> data);

		//parsing it
		void	take_off_default_setup(void);
		void	check_server_setup_duplicate(void);
		int		check_if_number(std::string data);
		void	check_status_code(std::string data);

		//getters
		std::vector<int> get_listen(void) const;
		std::vector<std::string> get_index(void) const;
		std::vector<std::string> get_server_name(void) const;
		std::vector<std::pair<int, std::string> >	get_status_page(void) const;
		std::vector<std::pair<std::string, std::string> >	get_cgi_info(void) const;
		std::string get_host(void) const;
		std::string get_root(void) const;
		std::string get_client_max_body_size(void) const;
		int	get_upload(void) const;
		int	get_autoindex(void) const;
		std::vector<std::string> get_allow_methods(void) const;
		std::vector<std::pair<int, std::string> >	get_retur(void) const;

		~Default_serv(void);
};
