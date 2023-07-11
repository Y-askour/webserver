#include "../include/Request.hpp"

Request::Request(Connection &other,int fd)
{
	std::cout << "constructor called" << std::endl;
	
	this->server = &(other.get_server());
	this->fd = fd;
}

Request::~Request()
{
	std::cout << "desctructor called " << std::endl;
}

Server& Request::get_server()
{
	return *this->server;
}

int Request::get_fd()
{
	return this->fd;
}
