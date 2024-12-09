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
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "cgiHandler.hpp"
#include "uploadHandler.hpp"
#include "deleteHandler.hpp"

#define CRLF "\r\n"

class postHandler{
    private:
        int _sock;

    public:
    postHandler();
    postHandler(int);
    void responde(request*);
    std::string actionDetector(request * entry);
    void execute(request * entry);
    std::string file_type(std::string route);
};