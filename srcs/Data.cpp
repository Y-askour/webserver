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
	std::string request;

	std::vector<Connection>::iterator it = this->connections.begin();
	while (it != this->connections.end())
	{
		assign_poll.fd = (*it).get_fd();
		assign_poll.events = POLLIN;
		this->mantained_fds.push_back(assign_poll);
		it++;
	}
	while (1)
	{
		int ret = poll(&this->mantained_fds[0],this->mantained_fds.size(),1000);
		
		if (ret == -1)
		{
			std::perror("webserv(poll)");
			return ;
		}
		
		size_t i = 0;
		while(i < this->mantained_fds.size())
		{
			if (this->mantained_fds[i].revents  & POLLHUP)
			{
				close(this->mantained_fds[i].fd);
				this->mantained_fds.erase(this->mantained_fds.begin() + i);
				continue;
			}
			else if (this->mantained_fds[i].revents & POLLIN)
			{
				if (this->is_a_connection(this->mantained_fds[i].fd))
				{
					socklen_t addr_size = sizeof client_struct;
					int client_fd = accept(this->mantained_fds[i].fd, &client_struct,&addr_size);
					Request req(*this->get_connection_by_fd(this->mantained_fds[i].fd),client_fd,this->mime_types_parse);
					this->req_res.push_back(req);

					if (client_fd < 0)
					{
						perror("webserv(accept)");
						continue;
					}
					assign_poll.fd = client_fd;
					assign_poll.events = POLLIN;
					this->mantained_fds[i].revents = 0;
					this->mantained_fds.push_back(assign_poll);
				}
				else 
				{
					Request *req = this->get_request_by_fd(this->mantained_fds[i].fd);

					char buf[1024];
					std::string	hld;
					size_t s = recv(this->mantained_fds[i].fd,buf, 1024, 0);
					for (size_t i = 0; i != s; i++)
						hld.push_back(buf[i]);
					if (s < 0)
					{
						close(this->mantained_fds[i].fd);
						this->delete_request(this->mantained_fds[i].fd);
						this->mantained_fds.erase(this->mantained_fds.begin() + i);
						continue;
					}
					if (!this->check) {
						request.append(hld);
						if (this->check_is_headers_done(request)) {
							req->parssing_the_request(request);
							request.clear();
						}
					}
					else
						req->parssing_the_request(hld);
					if (req->get_request_stat() == 2)
						this->mantained_fds[i].events = POLLOUT;
				}
			}
			else if (this->mantained_fds[i].revents & POLLOUT)
			{
				Request *req = this->get_request_by_fd(this->mantained_fds[i].fd);

				if (send(this->mantained_fds[i].fd,req->get_response_body().c_str(),req->get_response_body().length(),0) == -1)
					perror("webserv(send)");
				this->check = 0;
				close(this->mantained_fds[i].fd);
				request.clear();
				this->delete_request(this->mantained_fds[i].fd);
				this->mantained_fds.erase(this->mantained_fds.begin() + i);
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
