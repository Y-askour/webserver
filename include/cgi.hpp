/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrakibe <amrakibe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:23:31 by amrakibe          #+#    #+#             */
/*   Updated: 2023/07/19 17:35:33 by amrakibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Request.hpp"

using namespace std;

class CGI
{
    private:
        // std::string _body;
        // std::string _quryString;
        std::vector<std::string> _env;
        Request &_request;
        char **_av;
    public:
        CGI(Request &request);
        ~CGI();
        CGI(CGI const &other);

        CGI &operator=(CGI const &other);
        void setEnv();
        char  **_envToChar(std::vector<std::string> _env);
        void PERROR(std::string error);
        void getNameScript();
        bool isPython();
        vector<string> split(string line, char delimiter = ' ');
};
