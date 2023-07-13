#include "../include/Request.hpp"

Request::Request(Connection &other,int fd)
{
	this->server = &(other.get_server());
	this->fd = fd;
}

Request::~Request()
{
}

Server& Request::get_server()
{
	return *this->server;
}

int Request::get_fd()
{
	return this->fd;
}

void Request::set_request_buf(char *buf)
{
	this->request_buf = buf;
}

void Request::set_n_bytes(size_t n)
{
	this->n_bytes = n;
}

void Request::split_by_rclt()
{
	// getting the body
	size_t body_pos = this->request_buf.find("\r\n\r\n");
	this->body = this->request_buf.substr(body_pos + 4,this->request_buf.npos);
	this->request_buf.erase(body_pos + 4, this->request_buf.npos);

	std::vector<std::string> strs;
	size_t pos = 0;	

	// getting the headers and request line
	size_t i  = 0;
	while ((pos = this->request_buf.find("\r\n") ) != this->request_buf.npos)
	{
		if (i == 0)
			this->request_line = this->request_buf.substr(0,pos);
		else
			strs.push_back(this->request_buf.substr(0,pos));
		this->request_buf.erase(0,pos+2);
		i++;
	}
	strs.push_back(this->request_buf);

	// spliting headers and putting it in the private member headrs
	i = 0;
	while (i < strs.size())
	{
		if (strs[i].empty())
		{
			strs.erase(strs.begin() + i);
			continue;
		}
		std::pair<std::string,std::string> header;
		int pos = strs[i].find(':');
		header.first = strs[i].substr(0,pos);
		this->remove_spaces(header.first);
		strs[i].erase(0,pos + 1);
		this->remove_spaces(strs[i]);
		header.second = strs[i];
		//std::cout << header.first << ":" << header.second << std::endl;
		i++;
	}
}

void Request::remove_spaces(std::string &t)
{
	// delete space in the beginning
	std::string::iterator i = t.begin();
	while (i != t.end() )
	{
		if (!std::isspace(*i))
			break;
		i++;
	}
	t.erase(t.begin(), i);

	// delete spaces in the end
 	i = t.end() - 1;
	while (i >= t.begin() )
	{
		if (!std::isspace(*i))
			break;
		i--;
	}
	t.erase(i + 1,t.end());
}

