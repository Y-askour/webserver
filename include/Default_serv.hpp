/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Default_serv.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkaddour <hkaddour@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:34:30 by hkaddour          #+#    #+#             */
/*   Updated: 2023/06/19 10:06:47 by hkaddour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

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
		//constructers
		Default_serv(void);
		Default_serv(int);
		Default_serv(const Default_serv & obj);
		Default_serv & operator=(const Default_serv & obj);
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
		void	check_server_setup_duplicate(std::vector<int> &port_checking);
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

		//here maybe i will make a function with a string parameter for example "listen"
		//it returns 1 if exists and 0 if not to check if u will work with server var or location
		//function here that put status and path in a pair and adds it to status_page var.
		~Default_serv(void);
};
