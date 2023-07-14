#include "../include/Request.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
		this->headers.insert(header);
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

std::string Request::is_req_well_formed()
{
	std::map<std::string, std::string>::iterator it = this->headers.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator it1 = this->headers.find("Content-Length");

	if (it != this->headers.end() && it->second.compare("chuncked"))
		return ("501");
	else if (it == this->headers.end()  && it1 == this->headers.end() && !this->method.compare("POST"))
		return ("400");
	else if (!this->check_uri_characters())
		return ("400");
	else if (this->uri.size() > 2048)
		return ("414");
	else if (this->body.size() > (size_t)std::stoi(this->server->get_client_max_body_size()))
		return ("413");
	return ("");
}

int Request::check_uri_characters()
{
	std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";

	std::string::iterator it = this->uri.begin();
	it++;
	while (it != this->uri.end())
	{
		if (allowed_chars.find(*it) == allowed_chars.npos)
			return (0);
		it++;
	}
	return (1);
}

void Request::split_request_line()
{
	// removes spaces
	this->remove_spaces(this->request_line);

	std::string tmp = this->request_line;
	size_t i = 0;

	std::string strs[3];
	int k = 0;

	// split the request line parts
	while (i < tmp.size())
	{
		if (std::isspace(tmp[i]))
		{
			strs[k] = tmp.substr(0,i);
			tmp.erase(0,i + 1);
			i = 0;
			k++;
			continue;
		}
		i++;
	}
	strs[k] = tmp;

	this->method = strs[0];
	this->uri = strs[1];
	this->http_version = strs[2];
}

void Request::parssing_the_request(char *buf,size_t s)
{
	this->set_request_buf(buf);
	this->set_n_bytes(s);
	this->split_by_rclt();
	this->split_request_line();
	status = this->is_req_well_formed();
	std::cout << "|" << status <<  "|" << std::endl;
	if (status.empty())
	{
		std::pair<std::string,Default_serv*> status_location = this->get_matched_location_for_request();
		std::pair<int,std::string> t = this->is_Location_have_redirection(status_location.second);
		// no redirection
		if (!t.first)
		{
			status = this->is_method_allowed_in_location(status_location.second);
			if (status.empty())
			{
				// check what method
				return ;
			}
			// method is not allowed
			status = t.first;
			this->fill_status_line();
			this->fill_body(status_location.second,std::stoi(this->status));
			this->fill_headers();
			this->response = "";
			this->response += this->status_line; 
			this->response += this->response_headers;
			this->response += this->response_body + "\r\n";
			return ;
		}
		// there is a redirection
		status = t.first;
		this->fill_status_line();
		this->fill_body(status_location.second,std::stoi(this->status));
		this->fill_headers();
		this->response = "";
		this->response += this->status_line; 
		this->response += this->response_headers;
		this->response += this->response_body + "\r\n";
		return ;
	}
	this->fill_status_line();
	this->fill_body(this->server,std::stoi(this->status));
	this->fill_headers();
	this->response = "";
	this->response += this->status_line; 
	this->response += this->response_headers;
	this->response += this->response_body + "\r\n";
	std::cout << this->response << std::endl;

}
std::string Request::get_response_body()
{
	return this->response;
}

void Request::fill_headers()
{
	// i need to check what type of content
	this->response_headers += "Content-Type: text/html\r\n";
	this->response_headers += "Content-Length: " + std::to_string(this->response_body.length()) + "\r\n";
	this->response_headers += "\r\n";
}

void Request::fill_status_line()
{
	int status_code = std::stoi(this->status);
	this->status_line = "HTTP/1.1 " + this->status + " ";
	// 200
	if (status_code == 200)
		this->status_line += "OK\r\n";
	else if (status_code == 201)
		this->status_line += "Created\r\n";
	else if (status_code == 204)
		this->status_line += "No Content\r\n";
	// 300
	else if (status_code == 301)
		this->status_line += "Moved Permanently\r\n";
	// 400
	else if (status_code == 400)
		this->status_line += "Bad Request\r\n";
	else if (status_code == 403)
		this->status_line += "Forbidden\r\n";
	else if (status_code == 404)
		this->status_line += "Not Found\r\n";
	else if (status_code == 405)
		this->status_line += "Method Not Allowed\r\n";
	else if (status_code == 409) 
		this->status_line += "Conflict\r\n";
	else if (status_code == 413)
		this->status_line += "Request Entity Too Large\r\n";
	else if (status_code == 414)
		this->status_line += "Request-URI Too Long\r\n";
	// 500
	else if (status_code == 500)
		this->status_line += "Internal Server Error\r\n";
	else if (status_code == 501)
		this->status_line += "Not Implemented\r\n";
}
void Request::fill_body(Default_serv *serv,int status)
{
	if (status >=  400)
	{
		// getting the html and fill the body
		std::vector<std::pair<int,std::string> > t = serv->get_status_page();
		size_t i = 0;
		for (; i < t.size();i++)
		{
			if (status == t[i].first)
				break;
		}
		std::ifstream f(t[i].second);
		if (f.is_open())
		{
			std::ostringstream ss;
			ss << f.rdbuf();
			this->response_body = ss.str();
		}
	}
}

std::pair<std::string,Default_serv *> Request::get_matched_location_for_request()
{
	std::map<std::string, Location *> l = this->server->get_location();
	std::pair<std::string,Default_serv *> status_location;
	status_location.first = "/";
	status_location.second = this->server;
	std::map<std::string,Location *>::iterator re = l.find(this->uri);

	// find the location
	if (re != l.end())
	{
		status_location.first= re->first;
		status_location.second = re->second;
		return (status_location);
	}
	return (status_location);
}

std::pair<int,std::string> Request::is_Location_have_redirection(Default_serv * location)
{
	std::vector<std::pair<int,std::string> > redirection = location->get_retur();
	std::vector<std::pair<int,std::string> >::iterator it = redirection.begin();
	std::pair<int,std::string> no_location;
	no_location.first = 0;
	no_location.second = "";
	if (it != redirection.end())
		return *it;
	return no_location;
}

std::string Request::is_method_allowed_in_location(Default_serv *location)
{
	std::vector<std::string>t = location->get_allow_methods();
	std::vector<std::string>::iterator it = t.begin();
	while (it != t.end())
	{
		if (!it->compare(this->method))
			return "";
		it++;
	}
	return "405";
}


std::string Request::create_location()
{
	std::string root = this->server->get_root();
	return (0);
}
