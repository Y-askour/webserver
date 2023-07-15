#pragma once
#include "../include/Server.hpp"
#include "Connection.hpp"
#include "Default_serv.hpp"

#include <iostream>

class Request
{
	int fd;
	Server* server;
	std::string request_buf;
	size_t n_bytes;

	// request
	std::string request_line;
	std::map<std::string, std::string> headers;
	std::string body;
	
	std::string method;
	std::string uri;
	std::string http_version;

	// response
	std::string response;

	std::string status;

	std::string status_line;
	std::string response_headers;
	std::string response_body;

	public:
		Request(Connection& connection,int fd);
		~Request();

		// getters
		Server& get_server();
		int get_fd();

		// setters
		void set_request_buf(char *buf);
		void set_n_bytes(size_t n);

		// helpers
		void split_by_rclt();
		void remove_spaces(std::string &t);
		void split_request_line();
		int check_uri_characters();
		std::string create_location();

		// create response
		void parssing_the_request(char *buf,size_t s);
		std::string is_req_well_formed();
		std::pair<std::string,Default_serv *> get_matched_location_for_request();
		std::pair<int,std::string> is_Location_have_redirection(Default_serv * location);
		std::string is_method_allowed_in_location(Default_serv *location);
		void create_the_response();
		void fill_body(Default_serv *serv,int status);
		std::string get_response_body();
		void fill_status_line();
		void fill_headers(std::string location);

		// methods
		void GET_METHOD(Default_serv *serv);
};
