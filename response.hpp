
#pragma once

#include "struct.hpp"
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
        response(int, std::string, configuration);
        void fill_status_line(std::string);
        void fill_header();
        void redir_header(std::string);
        void fill_body_answer();
        void fill_body(std::string, configuration conf);
        //void sendToClient(std::string response, std::string location);
        std::string convertOutput();
        void readPage(std::string);
        void locationPage(std::string code, configuration conf);
        void reply(std::string, configuration conf);
        void reply(configuration);
        void redirect(std::string location, configuration conf);
};
