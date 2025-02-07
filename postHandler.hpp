#pragma once

#include "struct.hpp"
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
        //postHandler(int, std::map<std::string, std::string>);
        //void responde(request*, std::map<std::string, std::string> conf);
        void responde(request*, configuration conf);
        std::string actionDetector(request * entry);
        void execute(request * entry, configuration conf);
        std::string file_type(std::string route);
};
