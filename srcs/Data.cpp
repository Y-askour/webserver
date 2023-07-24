#include "../include/Data.hpp"
#include <cstdio>
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
	this->run_server();
}

std::vector<Server*> &Data::get_servers(void)
{
	return (this->servers);
}

void	Data::parse_file_and_syntax_error(void)
{
}

Data::~Data()
{
}

void Data::create_listen_sockets()
{
	std::vector<int> ports;
	std::vector<Server *>::iterator it = this->servers.begin();
	while (it != this->servers.end())
	{
		std::vector<int> serverPort = (*it)->get_listen();
		std::vector<int>::iterator n = serverPort.begin();
		while (n != serverPort.end())
		{
			Connection socket(*(*it),*n);
			this->connections.push_back(socket);
			n++;
		}
		it++;
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

	// test response /////////////////////////////

	std::ifstream f("./younes/www/html/index.html");
	std::string body = "<html>  <body> <h1>test<h1> </body> </html>\r\n";
	if (f)
	{
		std::ostringstream ss;
		ss << f.rdbuf();
		body = ss.str();
	}
	std::string msg = "HTTP/1.1 200 OK\r\n";
	msg += "Content-Type: text/html\r\n";
	msg += "Content-Length:" + std::to_string(body.length()) + "\r\n";
	msg += "\r\n";
	msg += body;
	///////////////////////////////

	while (1)
	{

		//std::cout << "hey" << std::endl;
		int ret = poll(&this->test[0],this->test.size(),1000);
		//std::cout << "blan : "  << ret << std::endl;
		
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
					// i need to make this part work with the Request class
					//Request *c = this->get_request_by_fd(this->test[i].fd);
					//std::cout << c->get_server().get_root() << std::endl;
					Request *younes = this->get_request_by_fd(this->test[i].fd);

					char buf[1024];
					std::string request;
				
					size_t s = recv(this->test[i].fd,buf,1024,0);
					if (s < 0)
					{
						close(this->test[i].fd);
						this->delete_request(this->test[i].fd);
						this->test.erase(this->test.begin() + i);
						continue;
					}
					buf[s] = 0;
					if (!this->check) {
						request.append(buf);
						if (this->check_is_headers_done(request))
							younes->parssing_the_request(request, s);
					}
					else
						younes->parssing_the_request(buf, s);
					if (younes->get_request_stat() == 2)
					{
						std::cout << "hey" << std::endl;
						this->test[i].events = POLLOUT;
					}
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
			std::cout << "ll" << std::endl;
		}
	}
}

int Data::get_number_of_clients()
{
	int i = 0;
	while (i < 10)
	{

		std::cout << "fd[" <<  i << "] :"<<  this->test[i].fd  << " " << this->test[i].revents << std::endl;
		i++;
	}
	return (i);
}

Server *Data::get_server_by_fd(int fd)
{
	size_t i = 0;
	while (i < this->connections.size())
	{
		if (this->connections[i].get_fd() == fd)
			return &this->connections[i].get_server();
		i++;
	}
	return NULL;
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
	for (size_t i = 0; i < this->connections.size(); i++)
	{
		if (this->connections[i].get_fd() == fd)
			return (1);
	}
	return 0;
}

int Data::check_is_headers_done(std::string request)
{
	//std::stringng hld;

	//for (size_t i = request.rfind('\n'); i != request.npos; i = request.rfind('\n')) {
	//	hld = request.substr();
	//	if (i >= 2) {
	//		if (request[i] == '')
	//	}
	//}

	if ( (request.find("\n\r\n") != request.npos) || (request.find("\n\n") != request.npos))
	{
		this->check  = 1;
		return 1;
	}

	return 0;
}
