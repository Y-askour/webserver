/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrakibe <amrakibe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:23:43 by amrakibe          #+#    #+#             */
/*   Updated: 2023/07/20 13:40:52 by amrakibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cgi.hpp"
#include <string>

vector<string> CGI::split(string line, std::string delimiter)
{
    vector<string> v;
    string sub_string;
    stringstream input_file(line);
    size_t npos;
    while ((npos = line.find(delimiter)) != string::npos)
    {
        sub_string = line.substr(0, npos);
        v.push_back(sub_string);
        line.erase(0,npos + 1);
    }
    v.push_back(line);
    return (v);
}

string body = "username=amrakibe&password=amine1337";

CGI::CGI(CGI const &other) : _request(other._request)
{
    *this = other;
}

CGI &CGI::operator=(CGI const &other)
{
    if (this != &other)
    {
        // _body = other._body;
        // _quryString = other._quryString;
        _env = other._env;
        _av = other._av;
    }
    return (*this);
}

CGI::CGI(Request &_request) : _request(_request)
{
    setEnv();
    isPython();
    getNameScript();
    string arr;
    char buf[4096] = {0};
    int pid, Pfd1[2], status = 0, fd_in = dup(0), fd_out = dup(1);
    size_t ret;
    string str;

    if (pipe(Pfd1) == -1 || this->_av == NULL)
    {
        _request.set_status_code("403");
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        _request.set_status_code("403");
        return;
    }

    if (pid == 0)
    {
        FILE *file = tmpfile();
        if (file == NULL)
        {
            _request.set_status_code("403");
            return;
        }

        fputs(body.c_str(), file);
        rewind(file);

        dup2(::fileno(file), 0);
        close(::fileno(file));

        dup2(Pfd1[1], 1);
        close(Pfd1[1]);

        if (execve(_av[0], _av, _envToChar(_env)) == -1)
        {
            cerr << "execve error" << endl;
            _request.set_status_code("403");
            return;
        }
    }
    waitpid(pid, &status, 0);
    if (status == -1)
    {
        _request.set_status_code("403");
        exit(EXIT_FAILURE);
    }

    close(Pfd1[1]);
    while ((ret = read(Pfd1[0], buf, sizeof(buf)) > 0))
    {
        // str += buf;
       std::vector<std::string> bufs = split(buf, "\r\n");
        for (size_t i = 0; i < bufs.size() ; i++)
        {
            std::cout << bufs[i] << std::endl;
        }
        _request.set_response_body(bufs[bufs.size() - 1]);
        _request.set_status_code("200");
    }
    close(Pfd1[0]);

    dup2(fd_in, 0);
    dup2(fd_out, 1);

    close(fd_in);
    close(fd_out);
    // cout << str << endl;
}

char **CGI::_envToChar(vector<string> _env)
{
    char **ret = (char **)malloc((_env.size() + 1) * sizeof(char *));
    size_t i = 0;

    for (; i < _env.size(); i++)
    {
        ret[i] = (char *)malloc((_env[i].size() + 1) * sizeof(char));
        strcpy(ret[i], _env[i].c_str());
        ret[i][_env[i].size()] = '\0';
    }
    ret[i] = NULL;
    return (ret);
}

void CGI::setEnv()
{
    _env.push_back("PATH_INFO=" + _request.get_file_path());
    _env.push_back("SERVER_SOFTWARE=webserv/1.0");
    _env.push_back("REMOTE_PORT=8080");
    _env.push_back("SERVR_NAME=localhost");
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SERVER_PORT=8081");
    _env.push_back("REQUEST_METHOD=" + _request.get_method());
    _env.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
    _env.push_back("QUERY_STRING=" + _request.get_query());
    _env.push_back("REDIRECT_STATUS=200");
    _env.push_back("SCRIPT_NAME=" + _request.get_file_root());
    _env.push_back("SCRIPT_FILENAME=" + _request.get_file_path());
    _env.push_back("CONTENT_LENGTH=" + to_string(body.length()));
    _env.push_back("DOCUMENT_ROOT=" + _request.get_file_root());
    _env.push_back("HTTP_COOKIE=first_name=amine&last_name=rakibe");
}

bool CGI::isPython()
{
    pair<string, string> _cgiPy = _request.get_cgi();
    for (size_t i = 0; i < _cgiPy.first.length(); i++)
    {
        if (_cgiPy.first == ".py")
            return (true);
    }
    return (false);
}

void CGI::getNameScript()
{
    vector<string> cgi_result;
    pair<string, string> arr = _request.get_cgi();
    string key;

    key = (isPython()) ? ".py" : ".php";

    if (arr.first.find(key) != string::npos)
        cgi_result.push_back(arr.second);

    if (cgi_result.empty())
    {
        _request.set_status_code("403");
        return;
    }

    this->_av = new char *[3];
    this->_av[0] = strdup(cgi_result[0].c_str());
    this->_av[1] = strdup(_request.get_file_path().c_str());
    this->_av[2] = NULL;
}

CGI::~CGI()
{
    for (size_t i = 0; this->_av[i]; i++)
    {
        delete this->_av[i];
    }
    delete this->_av;
}
