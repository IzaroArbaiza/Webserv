
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include "parse.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <sys/ioctl.h>
#include <csignal>
#include "request.hpp"
#include "getHandler.hpp"
#include "postHandler.hpp"
#include "deleteHandler.hpp"
#include "cgiHandler.hpp"
#include "response.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "RequestFactory.hpp"
#include <cstring>

#define CRLF "\r\n"

struct location_cgi {
    std::string cgi_root;
    std::string cgi_index; //ejecutable
    bool autoindex;
    bool getOn;
    bool postOn;
    bool deleteOn;
};

struct locationer {
    std::string location_name;
    std::string location_root;
    std::string index; //page
    bool autoindex;
    bool getOn;
    bool postOn;
    bool deleteOn;
};

typedef struct configuration {
    int         numServs;
    std::string server_name;
    std::string host;
    std::string port;
    std::string root;
    //std::string user = getenv("USER");
    std::string index;
    size_t      body_size;
    std::vector<locationer> locations;
    location_cgi cgi;
}configuration;

class cserver{
    private:
        std::map<std::string, std::string> config;
        bool get_allowed;
        bool post_allowed;
        bool delete_allowed;
        configuration my_config;

    public:
        
        void dostuff(int);
        cserver();
        cserver(configuration confis);
        void init_server(configuration);
        void init_server_default();
        std::string read_client (int sock);
        bool allowedMethod(int, std::string, std::map<std::string, std::string>);
        //server(info de config);
        ~cserver();  
};