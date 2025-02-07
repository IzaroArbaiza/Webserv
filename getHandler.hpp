#pragma once

#include "struct.hpp"
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
        std::string resource;

    public:
    getHandler();
    getHandler(int);
    //getHandler(int, std::map<std::string, std::string>);
    void responde(request * entry, configuration);
    //void responde(request * entry);
    bool isExecutable(request *, configuration);
    //std::string file_type(std::string route);
    std::string accesable(request * entry);
    void parse_resource(request * entry, configuration conf);
    void autoindex(request *entry, configuration);
    void index_in_folder(request *entry, configuration conf);
    void reload_location();
};
