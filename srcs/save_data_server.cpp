#include "../include/Parsing.hpp"

std::vector<std::pair<Parsing::t_tokens, std::string> >::iterator	Parsing::get_end_closing_braces(int)
{
	int	check;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	for (; this->begin->first != OPEN_BRACES && this->begin != this->tokens.end(); this->begin++)
		;
	for (this->begin++, iter = this->begin, check = 1; check && iter != this->tokens.end(); iter++)
	{
		if (iter->first == OPEN_BRACES)
			check++;
		else if (iter->first == CLOSE_BRACES)
			check--;
	}
	return (iter - 1);
}

void	Parsing::check_wish_directive(int check, std::vector<std::string> data)
{
	typedef void(Default_serv::*func)(std::vector<std::string>);
	func serv_func[] = {&Default_serv::set_listen, &Default_serv::set_host, \
		&Default_serv::set_server_name, &Default_serv::set_status_page, \
			&Default_serv::set_return, &Default_serv::set_root, &Default_serv::set_index, \
			&Default_serv::set_allow_methods, &Default_serv::set_client_max_body_size, \
			&Default_serv::set_autoindex, &Default_serv::set_cgi_info, &Default_serv::set_upload};
	for (int	i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		if (!this->directive_itr->second.compare(this->directive_name[i].second))
		{
			if (!check)
				((*(this->servers.end() - 1))->*serv_func[i])(data);
			else
				(*(std::prev((*(this->servers.end() - 1))->get_location().end()))->second.*serv_func[i])(data);
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
	unsigned long	size_location;
	Location	*location = new Location();
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;
	std::pair<std::string, Location*> hold((this->begin + 1)->second, location);
	this->servers_itr = std::prev(this->get_servers().end());

	size_location = (*(this->servers.end() - 1))->get_location().size();
	(*(this->servers.end() - 1))->set_locations(hold);
	if ((*this->servers_itr)->get_location().size() == size_location)
		throw ("Error: path of location is repeated.");
	iter = this->get_end_closing_braces(0);
	for (; this->begin != iter; this->begin++)
	{
		if (this->begin->first == DIRECTIVE)
			this->parse_directive(1);
	}
}

void	Parsing::parse_server(void)
{
	Server	*serv = new Server();

	this->servers.push_back(serv);
	this->end = this->get_end_closing_braces(0);
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
