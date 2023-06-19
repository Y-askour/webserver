#include "../include/Location.hpp"

Location::Location(void) : Default_serv(0) {
	//std::cout << "location constructer" << std::endl;
}

Location::Location(Default_serv const &obj) : Default_serv(obj)
{
	*this = obj;
}

Location & Location::operator=(Default_serv const & obj)
{
	(void)obj;
	//this->get_listen() = obj.get_listen();
	//this->get_server_name() = obj.get_server_name();
	//this->get_host() = obj.get_host();
	//this->get_index() = obj.get_index();
	//this->get_status_page() = obj.get_status_page();
	//this->get_cgi_info() = obj.get_cgi_info();
	//this->get_root() = obj.get_root();
	//this->get_client_max_body_size() = obj.get_client_max_body_size();
	//this->set_upload(obj.get_upload());
	//this->set_autoindex(obj.get_autoindex());
	//this->get_allow_methods() = obj.get_allow_methods();
	//this->get_retur() = obj.get_retur();
	return (*this);
}

Location::~Location(void) {
	//std::cout << "location destructer" << std::endl;
}
