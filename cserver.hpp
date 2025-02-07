#pragma once

#include "struct.hpp"
#include "request.hpp"
#include "getHandler.hpp"
#include "postHandler.hpp"
#include "deleteHandler.hpp"
#include "cgiHandler.hpp"
#include "response.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "RequestFactory.hpp"
#include "parseo/Parse.hpp"

#define CRLF "\r\n"

class cserver{
    private:
        //configuration conf;
        //std::map<std::string, std::string> error_pages;

    public:
        //void dostuff(int, std::map<std::string, std::string> );
        cserver();
        //cserver(configuration );
        void dostuff(int, configuration & );
        std::string read_client (int sock);
        bool allowedMethod(int, std::string, configuration);
        bool is_location(request *entry, configuration & config);
        void change_conf(int i, configuration & config);
        ~cserver();  
};
