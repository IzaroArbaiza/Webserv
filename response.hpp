
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

class response{
    private:
        std::string status_line;
        std::string header;
        std::string body;
        int socket;
        //request *entry;
        std::string code;
        std::string location;
        std::map<std::string, std::string> responseconfig;
    public:
        response();
        response(int);
        response(int, std::string);
        response(int, std::string, std::string);
        response(int, std::string, std::map<std::string, std::string>);
        void fill_status_line(std::string);
        void fill_header();
        void redir_header(std::string);
        void fill_body_answer();
        void fill_body(std::string);
        void sendToClient(std::string response, std::string location);
        std::string convertOutput();
        void readPage(std::string);
        void locationPage(std::string code);
        void reply(std::string);
        void reply();
        void redirect(std::string location);
};
