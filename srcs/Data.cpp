#include "../include/Data.hpp"
#include <cstdio>
#include <sys/poll.h>
#include <sys/socket.h>

Data::Data()
{
}

Data::Data(const Data & obj)
{
	this->mime_types_parse = obj.mime_types_parse;
	this->servers = obj.servers;
	this->create_listen_sockets();
	this->run_server();
}

std::vector<Server*> &Data::get_servers(void)
{
	return (this->servers);
}

void	Data::parse_file_and_syntax_error(void)
{
	std::cout << "d" << std::endl;
	std::cout << this->servers[0]->get_location().begin()->second->get_cgi_info()[0].first << std::endl;
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
	this->nb_clients =0;
	std::vector<Connection>::iterator it = this->connections.begin();
	while (it != this->connections.end())
	{
		this->poll_fd[nb_clients].fd = (*it).get_fd();
		this->poll_fd[nb_clients].events = POLLIN;
		nb_clients++;
		it++;
	}

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
	int a = 0;
	while (1)
	{
		//std::cout << "number of requests :"  << a  << " and open fds are : " << this->get_number_of_clients() << std::endl;
		std::cout << "hey" << std::endl;
		int ret = poll(this->poll_fd,this->nb_clients,-1);
		this->get_number_of_clients();
		std::cout << "---------------" << std::endl;
		if (ret == -1)
		{
			std::perror("webserv(poll)");
			return ;
		}
		for (int i = 0; i < this->nb_clients; i++)
		{
			if (this->poll_fd[i].revents == 0)
			{
				std::cout << "i = " <<  i  << "  |  nb_clients = " << this->nb_clients << std::endl;
				continue;
			}
			if (this->poll_fd[i].revents & POLLIN)
			{
				//std::cout << "recv" << std::endl;
				socklen_t addr_size = sizeof client_struct;
				int client_fd = accept(this->poll_fd[i].fd, &client_struct,&addr_size);
				if (client_fd < 0)
				{
					perror("webserv(accept)");
					return ;
				}
				char buf[1000011];
				ssize_t s = recv(client_fd,buf,1000000,0);
				buf[s] = 0;


				// add the client fd to the list
				this->poll_fd[nb_clients].fd = client_fd;
				this->poll_fd[nb_clients].events = POLLOUT;
				nb_clients++;
			}
			else if (this->poll_fd[i].revents & POLLOUT)
			{
				//std::cout << "send" << std::endl;
				if (send(this->poll_fd[i].fd,msg.c_str(),msg.length(),0) == -1)
				{
					perror("webserv(send)");
					return ;
				}
				close(this->poll_fd[i].fd);
				for (int j = i; j < this->nb_clients -1;j++)
				{
					this->poll_fd[j].fd = this->poll_fd[j + 1].fd;
					this->poll_fd[j].events = this->poll_fd[j + 1].events;
					this->poll_fd[j + 1].revents = this->poll_fd[j + 1].revents;
				}
				this->poll_fd[this->nb_clients -1].fd = 0;
				this->poll_fd[this->nb_clients -1].events = 0;
				this->poll_fd[this->nb_clients -1].revents = 0;
				nb_clients--;
				//std::cout << "send" << std::endl;
				a++;
			}
		}
	}
}

int Data::get_number_of_clients()
{
	int i = 0;
	while (i < 10)
	{

		std::cout << "fd[" <<  i << "] :"<<  this->poll_fd[i].fd  << " " << this->poll_fd[i].revents << std::endl;
		i++;
	}
	return (i);
}
