/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaskour <yaskour@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 14:00:07 by yaskour           #+#    #+#             */
/*   Updated: 2023/07/19 13:53:26 by yaskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"
#include <cctype>
#include <fstream>
#include <ostream>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <sys/unistd.h>
#include <vector>

Request::Request(Connection &other,int fd,std::map<std::string,std::string> mime)
{ this->server = &(other.get_server());
	this->fd = fd;
	this->mime_types = mime;
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
	else if (this->http_version.compare("HTTP/1.1"))
			return ("505");
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
	this->request_uri = strs[1];
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
	//std::cout << "|" << status <<  "|" << std::endl;
	if (status.empty())
	{
		Default_serv *location;
		// i don't need status_location.first
		std::pair<Server *,Default_serv*> status_location = this->get_matched_location_for_request();
		std::pair<int,std::string> t;

		if (status_location.second)
		{
			t = this->is_Location_have_redirection(status_location.second);
			location = status_location.second;
		}
		else 
		{
			t = this->is_Location_have_redirection(status_location.first);
			location = status_location.first;
		}
		// no redirection
		if (!t.first)
		{
			status = this->is_method_allowed_in_location(location);
			if (status.empty())
			{
				//std::cout << this->method << std::endl;
				if (this->method.compare("GET") == 0)
					this->GET_METHOD(status_location);
				else if (this->method.compare("DELETE") == 0)
				{
				}
				else if (this->method.compare("POST") == 0)
				{
				}
				else 
				{
				}
				// check what method
				return ;
			}
			// method is not allowed
			this->create_the_response();
			return ;
		}
		// there is a redirection
		this->status = std::to_string(t.first);
		this->create_the_response();
		return ;
	}
	this->create_the_response();
}

void Request::GET_METHOD(std::pair<Server* , Default_serv *>serv)
{
	// get path and file_type
	std::string root = serv.first->get_root();
	std::string second_root = "";
	Default_serv *location = serv.first;
	if (serv.second)
	{
		second_root = serv.second->get_root();
		location = serv.second;
	}
	if (!second_root.empty())
	{
		int ret = (root[root.size() -1] == '/');
		if ( ret && (second_root[0] == '/')  )
			root.erase(root.size() - 1);
		else if ( !ret  && !(second_root[0] == '/')  )
			root.push_back('/');
	}

	// path
	std::string path = root + second_root;
	if (path[path.size() -1] != '/' && this->uri.size())
		path.push_back('/');

	path += this->uri;
	path = this->find_path(path);


	int ret = access(path.c_str(),R_OK);
	if (ret == -1)
	{
		this->status = "404";
		this->create_the_response();
	}
	else 
	{
		struct stat buf;
		if (!stat(path.c_str(),&buf))
		{
			//std::cout << "this path :  " << path << " is  a : ";
			// directory 
			if (S_ISDIR(buf.st_mode))
			{
				if (this->request_uri[this->request_uri.size() - 1] == '/')
				{
					std::vector<std::string> indexs = location->get_index();

					// there is indexs
					if (indexs.size() > 0)
					{
						// check if there is a cgi

						std::vector<std::pair<std::string,std::string> > cgi = location->get_cgi_info();
						if (cgi.size() > 0)
						{
							// run cgi
							std::cout << path << std::endl;
							this->html_file = "/Users/yaskour/lwt/www/index.py";
							this->status = "201";
							this->create_the_response();
							return ;
						}
						// if there is not a cgi
						for (size_t i = 0; i < indexs.size(); i++)
						{
							std::string index_path;
							if (path[path.size() - 1] != '/')
								index_path = path + '/';
							index_path += indexs[i];
							int ret = access(index_path.c_str(),R_OK);
							if (!ret)
							{
								this->html_file = index_path;
								this->type_file = "text/html";
								this->status = "200";
								this->create_the_response();
								return ;
							}
						}
						this->status = "403";
						this->create_the_response();
						return ;
					}
					else
					{
						// get autoindex
						this->html_file = path;
						int check;
						check = location->get_autoindex();
						if (check == 1)
						{
							this->type_file = "text/html";
							this->create_auto_index();
							this->status = "200";
							this->fill_status_line();
							this->fill_headers();

							this->response = "";
							this->response += this->status_line; 
							this->response += this->response_headers;
							this->response += this->response_body + "\r\n";
						}
						else
						{
							this->status = "403";
							this->html_file = path;
							this->create_the_response();
						}

					}
				}
				else 
				{
					// redirect with the path with / in end
					this->status = "301";
					this->request_uri += '/';
					this->html_file = path;
					this->response_headers += "Location: " + this->request_uri + "\r\n";
					this->create_the_response();
				}
				//std::cout << "directory" << std::endl;
			}
			else {
				//std::cout << "file" << std::endl;
				std::vector<std::pair<std::string,std::string> > b = location->get_cgi_info();
				std::vector<std::pair<std::string,std::string> >::iterator t = b.begin();
				if (t != b.end())
				{
				}
				if (this->type_file.empty())
					this->type_file = "text/plain";
				this->status = "200";
				this->html_file = path;
				this->create_the_response();
			}

		}
	}
}

void Request::create_auto_index()
{
	//std::cout <<  this->html_file << std::endl;
	std::string auto_index = "<html><head><title>index</title></head><body><ul>";


	DIR *index = opendir(this->html_file.c_str());
	if (!index)
	{
		perror("opendir");
		return ;
	}
	struct dirent *entry;
	while ((entry = readdir(index)))
	{
		auto_index += "<li><a href=\"";
		auto_index.append(entry->d_name);
		auto_index += "\">";
		auto_index.append(entry->d_name);
		auto_index += "</a>";
	}
	closedir(index);
	auto_index += "</ul></body></html>";
	this->response_body = auto_index;
}

//std::vector<std::string,std::string> Request::split_ext(std::string ext);
std::vector<std::string>	Request::split_ext(std::string ext)
{
	std::vector<std::string> exts;
	for (size_t i = 0; i < ext.size(); i++)
	{
		if (std::isspace(ext[i]))
		{
			std::string splited_ext = ext.substr(0,i);
			ext.erase(0,i + 1);
			exts.push_back(splited_ext);
			i = 0;
			continue;
		}
	}
	return exts;
}

std::string Request::find_path(std::string path)
{
	size_t exetension = path.rfind('.');
	if (exetension != path.npos)
	{
		std::string ext = path.substr(exetension + 1,path.npos);
		this->type_of_file(ext,this->mime_types);
		return path;
	}
	else 
	{
		std::map<std::string,std::string>::iterator it = this->mime_types.begin();
		while (it != this->mime_types.end())
		{
			std::vector<std::string> exts = this->split_ext(it->second);
			for (size_t i = 0; i < exts.size(); i++)
			{
				std::string path_ext = path + "." + exts[i];
				if (!access(path_ext.c_str(),R_OK))
				{
					this->type_file = it->first;
					return path_ext;
				}
			}
			it++;
		}
		// check if it's python or php
		std::string path_ext = path + ".py";
		if (!access(path_ext.c_str(),R_OK))
		{
			this->type_file = "text/plain";
			return path_ext;
		}
		path_ext = path + ".php";
		if (!access(path_ext.c_str(),R_OK))
		{
			this->type_file = "text/plain";
			return path_ext;
		}
	}
	return path;
}

void Request::type_of_file(std::string ext,std::map<std::string,std::string> mime)
{
	std::map<std::string,std::string>::iterator it = mime.begin();

	while (it != mime.end())
	{
		std::vector<std::string> exts = this->split_ext(it->second);
		for (size_t i = 0; i < exts.size(); i++)
		{
			size_t pos;
			if ((pos = exts[i].find(ext))!= exts[i].npos && ( exts[i].size() == ext.size() ) )
			{
				this->type_file = it->first;
				return ;
			}
		}
		it++;
	}		
	return ;
}

std::string Request::get_response_body()
{
	return this->response;
}

void Request::fill_headers()
{
	// i need to check what type of content
	this->response_headers += "Content-Type: " + this->type_file + "\r\n";
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
	else if (status_code == 505)
		this->status_line += "Not Implemented\r\n";
}

void Request::fill_body(int status)
{
	if (status >=  300)
	{
		// getting the html and fill the body
		std::vector<std::pair<int,std::string> > t = this->server->get_status_page();
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
	else 
	{
		std::ifstream f(this->html_file);
		if (f.is_open())
		{
			std::ostringstream ss;
			ss << f.rdbuf();
			this->response_body = ss.str();
		}
	}
}


std::pair<Server *,Default_serv *> Request::get_matched_location_for_request()
{
	std::map<std::string, Location *> l = this->server->get_location();

	std::map<std::string, Location *>::iterator it = l.begin();
	while (it != l.end())
	{
		size_t yo = this->uri.find((*it).first,0);
		if (yo == 0)
			break;
		it++;
	}
	std::map<std::string,Location *>::iterator re = it;

	std::pair<Server *,Default_serv *> result;
	result.first = this->server;
	result.second = NULL;
	// find the location
	if (re != l.end())
	{
		this->uri.erase(0,(*re).first.size());
		int i = 0;
		while (this->uri[i]  && (this->uri[i] == '/') )
		{
			this->uri.erase(i,1);
		}

		i = this->uri.size() - 1;
		while (i >= 0 && this->uri[i] && (this->uri[i] == '/') )
		{
			this->uri.erase(i,1);
			i = this->uri.size() - 1;
		}
		result.second = re->second;
		return (result);
	}

	int i = 0;
	while (this->uri[i]  && (this->uri[i] == '/') )
	{
		this->uri.erase(i,1);
	}

	i = this->uri.size() - 1;
	while (i >= 0 && this->uri[i] && (this->uri[i] == '/') )
	{
		this->uri.erase(i,1);
		i = this->uri.size() - 1;
	}
	return (result);
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


void Request::create_the_response()
{
	this->fill_status_line();
	this->fill_body(std::stoi(this->status));
	this->fill_headers();
	this->response = "";
	this->response += this->status_line; 
	this->response += this->response_headers;
	this->response += this->response_body + "\r\n";
}
