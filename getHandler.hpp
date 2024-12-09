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
#include "request.hpp"
#include <fstream>
#include <bitset>
#include <stdbool.h>
#include <dirent.h>
#include "request.hpp"
#include "errorHandler.hpp"
#include "response.hpp"
#include "cgiHandler.hpp"
#include "deleteHandler.hpp"

#define CRLF "\r\n"
class cserver;

class getHandler{
    private:
        int _sock;
        std::map<std::string, std::string> gethandlerconfig;
        std::string resource;

    public:
    getHandler();
    getHandler(int);
    getHandler(int, std::map<std::string, std::string>);
    void responde(request * entry);
    bool isExecutable(request *);
    //std::string file_type(std::string route);
    std::string accesable(request * entry);
    void parse_resource(request * entry);
    void reload_location();
};