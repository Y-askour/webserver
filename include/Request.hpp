#pragma once
#include "../include/Server.hpp"
#include "Connection.hpp"
#include "Default_serv.hpp"
#include <string>
#include <unistd.h>
#include <sys/stat.h>


#include <iostream>

class Request
{
	int fd;
	Server* server;
	std::string request_buf;
	size_t n_bytes;

	// request
	std::string request_line;
	std::string request_uri;
	std::map<std::string, std::string> headers;
	std::string body;
	
	std::string method;
	std::string http_version;

	// response
	std::string response;

	std::string status;
	std::string uri;
	std::string status_line;

	// 3mer hna awld nass
	std::string response_headers;
	std::string response_body;
	// 

	std::string query;

	// cgi
	std::pair<std::string,std::string> cgi;


	// file to to read info
	std::string file_to_read;
	std::string file_type;
	std::string file_root;



	std::map<std::string,std::string> mime_types;
	public:
		Request(Connection& connection,int fd,std::map<std::string,std::string> mime);
		~Request();

		// getters
		Server& get_server();
		int get_fd();

		// amin getters
		std::string get_file_path();
		std::string get_method();
		std::string get_query();
		std::string get_file_root();
		std::string get_body();

		std::pair<std::string,std::string> get_cgi(); // first is the path of the script and the second is the extension of the script


		// setters
		void set_request_buf(char *buf);
		void set_n_bytes(size_t n);
		void set_response_body(std::string body);
		void set_response_headers(std::string headers);
		void set_status_code(std::string status);

		// helpers
		void split_by_rclt();
		void remove_spaces(std::string &t);
		void split_request_line();
		int check_uri_characters();

		// create response
		void parssing_the_request(char *buf,size_t s);
		std::string is_req_well_formed();
		std::pair<Server *,Default_serv *> get_matched_location_for_request();
		std::pair<int,std::string> is_Location_have_redirection(Default_serv * location);
		std::string is_method_allowed_in_location(Default_serv *location);
		

		void create_the_response();
		void fill_body(int status);
		std::string get_response_body();
		void fill_status_line();
		void fill_headers();

		// methods
		void	GET_METHOD(std::pair<Server *,Default_serv *>);
		void	POST_METHOD(std::pair<Server *,Default_serv *>);
		void	DELETE_METHOD(std::pair<Server *, Default_serv *> serv);

		// helpers
		void type_of_file(std::string path,std::map<std::string,std::string> mime);
		std::string find_type(std::string path);
		std::vector<std::string>	split_ext(std::string ext);
		void create_auto_index();
		int location_support_upload(Default_serv *location);
		std::string get_requested_resource(std::pair<Server *,Default_serv *> serv,Default_serv **);
		void check_index_files(Default_serv *);
		void check_cgi(Default_serv *,std::string path_with_index);
		void join_reponse_parts();
};
