#pragma once

#include "Default_serv.hpp"
#include <iostream>
#include <vector>

class	Location : public Default_serv
{
	public :
		Location(void);
		//Location(const Default_serv &obj)
		Location(Default_serv &obj)
		{
			*this = obj;
		}

		//here i have to use const
		Location & operator=(Default_serv & obj)
		{
			//here check if size is bigger than one not here let it to the end
			//this one only set in the server not the location
			this->get_listen() = obj.get_listen();
			this->get_server_name() = obj.get_server_name();
			this->get_host() = obj.get_host();
			//
			this->get_index() = obj.get_index();
			this->get_status_page() = obj.get_status_page();
			this->get_cgi_info() = obj.get_cgi_info();
			this->get_root() = obj.get_root();
			this->get_client_max_body_size() = obj.get_client_max_body_size();
			//this->upload = obj.get_upload();
			*(this->get_upload()) = *obj.get_upload();
			*(this->get_autoindex()) = *obj.get_autoindex();
			//this->autoindex = obj.get_autoindex();
			this->get_allow_methods() = obj.get_allow_methods();
			this->get_retur() = obj.get_retur();
			return (*this);
		}
		~Location(void);
};
