#include "../include/Request.hpp"
#include <cctype>
#include <fstream>
#include <iterator>
#include <ostream>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <vector>
#include "../include/cgi.hpp"

Request::Request(Connection &other,int fd,std::map<std::string,std::string> mime)
{
	this->server = &(other.get_server());
	this->fd = fd;
	this->mime_types = mime;
	this->request_stat  = 0;
	this->n_bytes = 0;
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

void Request::set_request_buf(std::string buf)
{
	this->request_buf = "";
	for (size_t i = 0; i < this->n_bytes ; i++)
		this->request_buf.push_back(buf[i]);
}

void Request::set_n_bytes(size_t n)
{
	this->n_bytes += n;
}


void Request::remove_spaces_at_end(std::string &t)
{
	std::string::iterator i = t.begin();
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

void	Request::is_req_well_formed()
{
	std::map<std::string, std::string>::iterator it = this->headers.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator it1 = this->headers.find("Content-Length");

	//if (!this->bad_request.compare("1"))
	//	return ("400");
	if (it != this->headers.end() && it->second.compare("chuncked") != 0)
		throw ("501");
	else if (it == this->headers.end() && it1 == this->headers.end() && !this->method.compare("POST"))
		throw ("400");
	//this one is wrong 
	//else if (!this->check_uri_characters())
	//	return ("400");
	//else if (this->uri.size() > 2048)
	//	return ("414");
	//else if (this->http_version.compare("HTTP/1.1"))
	//		return ("505");
	//return ("");
}

int Request::check_uri_characters()
{
	std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";

	std::string::iterator it = this->uri.begin();

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
	std::vector<std::string> strs;
	// removes spaces
	if (this->request_line.empty() || (this->request_line[0] == ' '))
	{
		this->bad_request = "1";
		return;
	}
	this->remove_spaces_at_end(this->request_line);

	// split the request line parts
	strs = this->split(this->request_line,' ');
	if (strs.size() != 3)
	{
		this->bad_request = "1";
		return ;
	}

	this->method = strs[0];
	size_t pos = strs[1].find("?");
	if (pos != strs[1].npos)
	{
		this->query = strs[1].substr(pos + 1);
		strs[1].erase(pos,strs[1].npos);
	}
	this->request_uri = strs[1];
	this->uri = strs[1];
	this->http_version = strs[2];
}

std::vector<std::string>    Request::split(std::string input, char sp) {
    std::vector<std::string>    header;

    for (unsigned long i = input.find(sp); input.length(); i = input.find(sp)) {
        if (i == std::string::npos) {
            header.push_back(input.substr(0, input.length()));
            break ;
        }
        header.push_back(input.substr(0, i));
        while (input.at(i) == sp)
            i++;
        input.erase(0, i);
    }
    return header;
}

void Request::remove_spaces_at_start(std::string &str) 
{
	std::string::iterator it = str.begin();

	while (it != str.end())
	{
		if (!std::isspace(*it))
			break;
		else
		 	str.erase(it);
		it++;
	}
}

std::string Request::turn_whitespaces_to_space(std::string input)
{
	for (int	i = 0; input[i]; i++)
	{
		if (isspace(input.c_str()[i]))
			input[i] = ' ';
	}
	return input;
}

//parsing request func

bool	Request::check_uri_character(char c) {
	std::string	allow = URI;
	if (allow.find(c) == std::string::npos)
		return 1;
	return 0;
}

void	Request::parse_request_line(void) {
	std::vector<std::string>	req;
	unsigned long	pos;
	std::string	hld;

	pos = this->request_buf.find('\n');
	hld = this->request_buf.substr(0, pos);
	this->request_buf.erase(0, pos + 1);
	if (hld.back() == '\r')
		hld.erase(hld.length() - 1);
	while (hld.back() == ' ')
		hld.erase(hld.length() - 1);
	if (hld.at(0) == ' ')
		throw "400";
	req = this->split(hld, ' ');
	if (req.size() > 3)
		throw "400";
	this->method = req.at(0);
	//here take off the query ?ll=lll
	pos = (req.at(1).find('?') != req.at(1).npos) ? req.at(1).find('?') : req.at(1).length();
	this->uri = req.at(1).substr(0, pos);
	this->request_uri = req.at(1).substr(0, pos);
	req.at(1).erase(0, ((req.at(1)[pos] == '?') ? pos + 1 : pos));

	this->query = req.at(1).substr(0, req.at(1).length());
	//if (req.at(1).find('?') != req.at(1).npos)
	//	this->query = req.at(1).substr(pos + 1, req.at(1).length());
	//else
	//	this->query = "";
	//this->query = (pos != std::string::npos) ? req.at(1).substr(pos + 1, req.at(1).length()) : "";
	this->http_version = req.at(2);
	//check if method is correct
	if (this->method.compare("GET") && this->method.compare("POST") && this->method.compare("DELETE"))
		throw "400";
	//here check url uncoding if character is right
	if (this->uri.front() != '/')
		throw "400";
	if (this->uri.length() > 2048)
		throw "414";
	//here also check ? and save the query of the uri
	for (std::string::iterator itr = this->uri.begin(); itr != this->uri.end(); itr++) {
		if (check_uri_character(*itr))
			throw "400";
	}
	//here youness should check if http/1.1
	if (this->http_version.compare("HTTP/1.1"))
		throw "400";

}

void	Request::check_header_variables(void) {
	std::map<std::string, std::string>::iterator	itr = this->headers.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator	itr2 = this->headers.find("Content-Length");

	if (itr != this->headers.end() && itr->second.compare("chuncked"))
		throw "501";
	if (!this->method.compare("POST") && itr2 == this->headers.end())
		throw "400";
}

std::string	Request::substr_sp(std::string path, char sp) {
	size_t	i;
	size_t	j;

	for (i = 0; (i < (path.length() -1) && path.at(i) == sp);)
		i++;
	for (j = (path.length() - 1); ( (path.length() -1) && path.at(j) == sp);)
		j--;
	return (path.substr(i, (j - i) + 1));
}

void	Request::parse_header(void) {
	std::vector<std::string>	header;
	std::string	hld;
	int	check = 0;
	unsigned long find;

	for (find = this->request_buf.find('\n'); !this->request_buf.empty(); find = this->request_buf.find('\n')) {
		hld = this->request_buf.substr(0, find + 1);
		if ((hld.length() == 2 && hld.at(0) == '\r') || hld.length() == 1) {
			this->request_buf.erase(0, find + 1);
			check = 1;
			break ;
		}
		header.push_back(hld);
		this->request_buf.erase(0, find + 1);
	}
	if (!check)
		throw "400";
	for (std::vector<std::string>::iterator	itr = header.begin(); itr != header.end(); itr++) {
		find = itr->find(':');
		if (find == std::string::npos)
			throw "400";
		itr->erase(itr->length() - 1);
		if (itr->back() == '\r')
			itr->erase(itr->length() - 1);
		//std::pair<std::string, std::string>	var(itr->substr(0, find), itr->substr(find + 1, itr->length() - 1));
		std::pair<std::string, std::string>	var(itr->substr(0, find), \
				this->substr_sp(itr->substr(find + 1, itr->length() - 1), ' '));
		for (std::string::iterator i = var.first.begin(); i != var.first.end(); i++) {
			if (isspace(*i))
				throw "400";
		}
		this->headers.insert(var);
	}
	//here check
	this->check_header_variables();
}

void	Request::parse_body(void) {
	std::map<std::string, std::string>::iterator itr = this->headers.find("Content-Length");
	if (this->method.compare("GET") == 0)
	{
		this->body = this->request_buf;
		this->request_stat = 2;
	}
	else if (itr != this->headers.end())
	{
		this->body.append(this->request_buf);
		if ((size_t)(std::atoi(itr->second.c_str())) <= this->body.size())
			this->request_stat = 2;
	}
	//this one is wrong is younes
	//if (this->body.size() > (size_t)std::stoi(this->server->get_client_max_body_size()))
	//	throw ("413");
	//this one have a problem maybe the size will be long i need to check it in the parsing atoi will not do the job
	if (static_cast<int>(this->body.length()) > atoi(server->get_client_max_body_size().c_str()))
		throw "413";
}

void Request::parssing_the_request(std::string buf,size_t s)
{
	//hicham code
	try {
		this->set_n_bytes(s);
		this->set_request_buf(buf);
		if (!this->request_stat) 
		{
			this->parse_request_line();
			this->parse_header();
			this->request_stat = 1;
		}
		this->parse_body();
		if (this->request_stat != 2)
			return ;
		//this->is_req_well_formed();
	}
	catch (const char *status) {
		this->status = status;
	}
	// request flow 
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
			this->status = this->is_method_allowed_in_location(location);
			if (status.empty())
			{
				//std::cout << this->method << std::endl;
				if (this->method.compare("GET") == 0)
					this->GET_METHOD(status_location);
				else if (this->method.compare("POST") == 0)
					this->POST_METHOD(status_location);
				else if (this->method.compare("DELETE") == 0)
					this->DELETE_METHOD(status_location);
				else 
				{
					this->status = "501";
					this->create_the_response();
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
		this->response_headers = "Location: " + t.second  + "\r\n";

		// i need to add location header
		this->create_the_response();
		return ;
	}
	this->create_the_response();
}

void Request::GET_METHOD(std::pair<Server* , Default_serv *>serv)
{
	// get path and file_type
	Default_serv *location;
	this->get_requested_resource(serv,&location);

	int ret = access(this->file_to_read.c_str(),R_OK);

	if (ret == -1)
	{
		this->status = "404";
		this->create_the_response();
	}
	else 
	{
		struct stat buf;
		if (!stat(file_to_read.c_str(),&buf))
		{
			// directory 
			if (S_ISDIR(buf.st_mode))
			{
				if (this->request_uri[this->request_uri.size() - 1] == '/')
				{
					std::vector<std::string> indexs = location->get_index();
					this->check_index_files(location);
				}
				else 
				{
					// this is a directory without a / in end
					// redirect with the path with / in end
					this->status = "301";
					this->request_uri += '/';
					this->response_headers += "Location: " + this->request_uri + "\r\n";
					this->create_the_response();
				}
			}
			else {
				std::vector<std::pair<std::string,std::string> > cgi = location->get_cgi_info();
				std::vector<std::pair<std::string,std::string> >::iterator t = cgi.begin();
				if (t != cgi.end())
					return this->check_cgi(location,this->file_to_read);
				this->find_type(this->file_to_read);
				std::cout << this->file_type << std::endl;
				this->status = "200";
				this->create_the_response();
			}

		}
	}
}

int Request::location_support_upload(Default_serv *location)
{
	int check = location->get_upload();
	return check;
}

void Request::POST_METHOD(std::pair<Server *,Default_serv *> serv)
{
	Default_serv *location;
	if (serv.second)
		location = serv.second;
	else
		location = serv.first;
	if (this->location_support_upload(location) == 1)
	{
		// upload the post request body
		std::cout << "on" << std::endl;
		this->status = "404";
		this->file_type = "text/html";
		this->create_the_response();
		return ;
	}
	this->get_requested_resource(serv,&location);
	int ret = access(this->file_to_read.c_str(),R_OK);
	// not found
	if (ret == -1)
	{
		this->status = "404";
		this->create_the_response();
	}
	if (!ret)
	{
		struct stat buf;
		if (!stat(file_to_read.c_str(),&buf))
		{
			if (S_ISDIR(buf.st_mode)) // directory
			{
				if (this->request_uri[this->request_uri.size() - 1] == '/')
				{
					std::vector<std::string> indexs = location->get_index();
					this->check_index_files(location);
				}
				else
				{
					this->status = "301";
					this->request_uri += '/';
					this->response_headers += "Location: " + this->request_uri + "\r\n";
					this->create_the_response();
				}
			}
			else // file
			{
				std::vector<std::pair<std::string,std::string> > b = location->get_cgi_info();
				std::vector<std::pair<std::string,std::string> >::iterator t = b.begin();
				if (t != b.end())
					return this->check_cgi(location,this->file_to_read);
				this->status  = "403";
				this->create_the_response();
			}

		}
	}
}

int Request::delete_all_folder_content()
{
	DIR *folder = opendir(this->file_to_read.c_str());
	if (!folder)
	{
		perror("opendir");
		return 0;
	}
	struct dirent *entry;
	while ((entry = readdir(folder)))
	{
		std::string file_name = entry->d_name;
		std::string path = this->file_to_read  + "/" + entry->d_name ;
		if (file_name.compare("..") && file_name.compare("."))
		{
	   		if (std::remove(path.c_str()))
	   		{
	   			closedir(folder);
	   			return 1;
	   		}
		}
	}
	closedir(folder);
	return 0;
}

void	Request::DELETE_METHOD(std::pair<Server *, Default_serv *> serv)
{
	Default_serv	*location;
	if (serv.second)
		location = serv.second;
	else
		location = serv.first;

	this->get_requested_resource(serv, &location);

	int ret = access(this->file_to_read.c_str(),R_OK);

	if (ret == -1)
	{
		this->status = "404";
		this->create_the_response();
	}
	else
	{
		struct stat buf;
		if (!stat(file_to_read.c_str(),&buf))
		{
			if (S_ISDIR(buf.st_mode)) // directory
			{
				if (this->request_uri[this->request_uri.size() - 1] == '/')
				{
					std::vector<std::pair<std::string,std::string> > b = location->get_cgi_info();
					if (b.size() > 0)
						this->check_index_files(location);
					else
					{
						int check = this->delete_all_folder_content();
						if (!check)
						{
							this->status = "204";
							this->create_the_response();
							return ;
						}
						else
						{
							this->status = "403";
							this->create_the_response();
						}
					}
				}
				else
				{
					this->status = "409";
					this->create_the_response();
				}

			}
			else // file
			{
				std::vector<std::pair<std::string,std::string> > b = location->get_cgi_info();
				std::vector<std::pair<std::string,std::string> >::iterator t = b.begin();
				if (t != b.end())
					return this->check_cgi(location,this->file_to_read);
				else if (std::remove(this->file_to_read.c_str()))
				{
					this->status = "403";
					this->create_the_response();
	   				return ;
				}
				this->status = "204";
				this->create_the_response();
			}
		}

	}
}

std::string Request::get_requested_resource(std::pair<Server *,Default_serv *> serv,Default_serv **location)
{
	std::string root = serv.first->get_root();
	std::string second_root = "";
	*location = (serv.first);
	if (serv.second)
	{
		second_root = serv.second->get_root();
		*location = serv.second;
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

	this->file_root = path;
	path += this->uri;

	path = this->find_type(path);
	this->file_to_read = path;
	return path;
	//why the fuck did u return in here
}



void Request::create_auto_index()
{
	//std::cout <<  this->file_to_read << std::endl;
	std::string auto_index = "<html><head><title>index</title></head><body><ul>";


	DIR *index = opendir(this->file_to_read.c_str());
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
			ext.erase(0, i + 1);
			exts.push_back(splited_ext);
			i = 0;
			continue;
		}
	}
	return exts;
}

std::string Request::find_type(std::string path)
{
	this->file_type = "text/plain";
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
					this->file_type = it->first;
					return path_ext;
				}
			}
			it++;
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
				this->file_type = it->first;
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
	this->response_headers += "Content-Type: " + this->file_type + "\r\n";
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
	if ( status == 204 || status >=  300)
	{
		// getting the html and fill the body
		this->file_type = "text/html";
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
		std::ifstream f(this->file_to_read);
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
	if (it == t.end())
		return "";
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


std::string Request::get_file_path()
{
	return (this->file_to_read);
}

std::string Request::get_method()
{
	return (this->method);
}

std::string Request::get_query()
{
	return (this->query);
}

std::string Request::get_file_root()
{
	return (this->file_root);
}

std::pair<std::string,std::string> Request::get_cgi()
{
	return (this->cgi);
}

void Request::set_response_body(std::string body)
{
	this->response_body = body;
	cout << "body : " << this->response_body << endl;
}

void Request::set_response_headers(std::string headers)
{
	if (this->response_headers.length() == 0)
		this->response_headers = headers;
	else {
		this->response_headers += "\r\n" + headers;
	}
}

void Request::set_status_code(std::string status)
{   
	this->status = status;
}

void Request::check_cgi(Default_serv *location,std::string file)
{
	std::vector<std::pair<std::string,std::string> > cgi = location->get_cgi_info();
	size_t end = file.npos;
	if ( ( (file.find(".py") != end) || (file.find(".php") != end) ) && cgi.size() > 0 )
	{
		for (size_t i = 0; i < cgi.size();i++)
		{
			if (file.find(cgi[i].first) != end)
			{
				this->status = "200";
				this->file_to_read = file;
				this->cgi = cgi[i];
				// i need to check cgi errors
				CGI aa(*this);
				if (status.compare("200"))
					return this->create_the_response();
				this->file_type = "text/html";
				this->fill_status_line();
				this->fill_headers();
				this->join_reponse_parts();
				return ;
			}
		}
	}
	this->status = "403";
	if (!this->method.compare("GET"))
	{
		this->status = "200";
		this->file_to_read = file;
		this->find_type(this->file_to_read);
	}
	this->create_the_response();
}

void Request::check_index_files(Default_serv *location)
{
	std::vector<std::string> indexs = location->get_index();
	if (indexs.size() > 0)
	{
		std::string path_with_index;
		for (size_t i = 0; i < indexs.size(); i++)
		{
			if (this->file_to_read[file_to_read.size() - 1] != '/')
				path_with_index = file_to_read + '/';
			else
				path_with_index = file_to_read;
			path_with_index += indexs[i];
			if (!access(path_with_index.c_str(), R_OK))
				return check_cgi(location,path_with_index);
		}
		// no index files
		this->status = "403";
		this->create_the_response();
	}
	else if (!this->method.compare("GET"))
	{
		int is_autoindex = location->get_autoindex();
		if (is_autoindex == 1)
		{
			this->file_type = "text/html";
			this->create_auto_index();
			this->status = "200";
			this->fill_status_line();
			this->fill_headers();
			this->join_reponse_parts();
		}
		else
		{
			this->status = "403";
			this->create_the_response();
		}
	}
	else
	{
		this->status = "403";
		this->create_the_response();
	}
}

void Request::join_reponse_parts()
{
	this->response = "";
	this->response += this->status_line; 
	this->response += this->response_headers;
	this->response += this->response_body + "\r\n";
}

std::string Request::get_body()
{
	return (this->body);
}

size_t Request::get_request_stat()
{
	return this->request_stat;
}
