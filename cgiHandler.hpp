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
#include <sys/wait.h>
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "response.hpp"
#include <sys/wait.h>
#include <sys/types.h>


class cgiHandler{
    private:
        int _sock;
        std::map<std::string, std::string> cgiconfig;
        std::string resource;
        //request * entry; copiar al iniciar y ya no necesitamos pasar a funciones
    public:
    cgiHandler();
    cgiHandler(int);
    cgiHandler(int, std::map<std::string, std::string>);
    //cgiHandler(int, request *);
    void responde(request*);
    void execBinary(std::string file_name, request*);
    void execPy(std::string file_name, request*);
    std::string cgiFileName(request * entry);
    std::string vblesMethod(request * entry);
    std::string buildPath(request * entry);
    std::string envPath(request * entry);
    void parse_resource(request * entry);
};