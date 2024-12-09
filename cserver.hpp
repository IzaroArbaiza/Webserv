
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

class cserver{
    private:
        std::map<std::string, std::string> config;
        bool get_allowed;
        bool post_allowed;
        bool delete_allowed;

    public:
        
        void dostuff(int);
        cserver();
        void init_server();
        std::string read_client (int sock);
        bool allowedMethod(int, std::string, std::map<std::string, std::string>);
        //server(info de config);
        ~cserver();  
};