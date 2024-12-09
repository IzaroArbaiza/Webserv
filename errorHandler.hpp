#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <stdlib.h>

#include <stdio.h>

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
#include "request.hpp"
#include <fstream>
#include <bitset>
#include "request.hpp"
#include "errorHandler.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class errorHandler{
    private:
        std::string _request_line;
        std::string error_code;
        std::string uri;
        std::string answer;

    public:
        std::map<std::string, std::string> _header; //each line is a header field <name:> <value>
        int socket;
        std::string rawRequest;
        std::string request_body;
        errorHandler (std::string, int);
        errorHandler ();
        errorHandler (errorHandler&);
        errorHandler& operator=(errorHandler&);
        ~errorHandler();
        std::string getErrorCode();
        std::string getUri();
        std::string getAnswer();
        std::string errorHead(std::string);
        std::string errorPage(std::string);
        std::string errorReply();
};




