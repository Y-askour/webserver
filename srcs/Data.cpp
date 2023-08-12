#include "../include/Data.hpp"
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>

Data::Data()
{
}

Data::Data(const Data & obj)
{
	this->mime_types_parse = obj.mime_types_parse;
	this->servers = obj.servers;
	this->check = 0;
	this->create_listen_sockets();
	if (this->connections.size() > 0)
		this->run_server();
}

std::vector<Server*> &Data::get_servers(void)
{
	return (this->servers);
}


Data::~Data()
{
}

void Data::create_listen_sockets()
{
	std::vector<std::pair<int,std::string> > ports;
	std::vector<Server *>::iterator it = this->servers.begin();
	while (it != this->servers.end())
	{
		std::pair<int,std::string> port_host;
		std::vector<int> serverPort = (*it)->get_listen();
		std::vector<int>::iterator n = serverPort.begin();
		port_host.second = (*it)->get_host();
		while (n != serverPort.end())
		{
			port_host.first = *n;
			ports.push_back(port_host);
			n++;
		}
		it++;
	}

	// delete duplicates
	std::vector<std::pair<int,std::string> >::iterator h_it = ports.begin();
	std::vector<std::pair<int,std::string> >::iterator h_it2;

	size_t i = 0;
	while (i < (ports.size() - 1))
	{
		h_it2 = h_it;
		h_it2++;
		while (h_it2 != ports.end())
		{
			if (h_it->first == h_it2->first && h_it->second == h_it2->second)
			{
				ports.erase(h_it2);
				continue;
			}
			h_it2++;
		}
		h_it++;
		i++;
	}

	for (std::vector<std::pair<int,std::string> >::iterator it = ports.begin();it != ports.end();it++)
	{
		Connection socket(this->servers,*it);
		if (socket.get_fd() != -1)
			this->connections.push_back(socket);
	}
}

void Data::run_server()
{
	struct sockaddr client_struct;
	struct pollfd assign_poll;

	std::vector<Connection>::iterator it = this->connections.begin();
	while (it != this->connections.end())
	{
		assign_poll.fd = (*it).get_fd();
		assign_poll.events = POLLIN;
		this->test.push_back(assign_poll);
		it++;
	}
	while (1)
	{
		int ret = poll(&this->test[0],this->test.size(),1000);
		
		if (ret == -1)
		{
			std::perror("webserv(poll)");
			return ;
		}
		
		size_t i = 0;
		while(i < this->test.size())
		{
			if (this->test[i].revents  & POLLHUP)
			{
				close(this->test[i].fd);
				this->test.erase(this->test.begin() + i);
				continue;
			}
			else if (this->test[i].revents & POLLIN)
			{
				if (this->is_a_connection(this->test[i].fd))
				{
					socklen_t addr_size = sizeof client_struct;
					int client_fd = accept(this->test[i].fd, &client_struct,&addr_size);
					Request req(*this->get_connection_by_fd(this->test[i].fd),client_fd,this->mime_types_parse);
					this->req_res.push_back(req);

					if (client_fd < 0)
					{
						perror("webserv(accept)");
						return ;
					}
					assign_poll.fd = client_fd;
					assign_poll.events = POLLIN;
					this->test[i].revents = 0;
					this->test.push_back(assign_poll);
				}
				else 
				{
					Request *younes = this->get_request_by_fd(this->test[i].fd);

					char buf[1024];
					std::string	hld;
					std::string request;
					size_t s = recv(this->test[i].fd,buf, 1024, 0);
					for (size_t i = 0; i != s; i++)
						hld.push_back(buf[i]);
					if (s < 0)
					{
						close(this->test[i].fd);
						this->delete_request(this->test[i].fd);
						this->test.erase(this->test.begin() + i);
						continue;
					}
					if (!this->check) {
						request.append(hld);
						if (this->check_is_headers_done(request))
							younes->parssing_the_request(request, s);
					}
					else
						younes->parssing_the_request(hld, s);
					if (younes->get_request_stat() == 2)
						this->test[i].events = POLLOUT;
				}
			}
			else if (this->test[i].revents & POLLOUT)
			{
				Request *younes = this->get_request_by_fd(this->test[i].fd);
				// i need to make this part work with the Request class

				if (send(this->test[i].fd,younes->get_response_body().c_str(),younes->get_response_body().length(),0) == -1)
				{
					perror("webserv(send)");
					return ;
				}
				close(this->test[i].fd);
				this->delete_request(this->test[i].fd);
				this->test.erase(this->test.begin() + i);
				continue;
			}
			i++;
		}
	}
}


Connection *Data::get_connection_by_fd(int fd)
{
	for (size_t i  = 0 ; i < this->connections.size(); i++)
	{
		if (this->connections[i].get_fd() == fd)
			return &(this->connections[i]);
	}
	return 0;
}

Request *Data::get_request_by_fd(int fd)
{
	for (size_t i = 0 ; i < this->req_res.size(); i++)
	{
		if (this->req_res[i].get_fd() == fd)
			return &(this->req_res[i]);
	}
	return 0;
}
void Data::delete_request(int fd)
{
	for (size_t i = 0; i < this->req_res.size(); i++)
	{
		if (this->req_res[i].get_fd() == fd)
		{
			this->req_res.erase(this->req_res.begin() + i);
		}
	}
	return ;
}

bool Data::is_a_connection(int fd)
{
	for (size_t i = 0; i < this->connections.size(); i++) {
		if (this->connections[i].get_fd() == fd)
			return (1);
	}
	return 0;
}

int Data::check_is_headers_done(std::string request)
{
	if ( (request.find("\n\r\n") != request.npos) || (request.find("\n\n") != request.npos)) {
		this->check = 1;
		return 1;
	}
	return 0;
}
