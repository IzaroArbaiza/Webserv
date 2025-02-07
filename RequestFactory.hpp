#pragma once

#include "struct.hpp"
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "deleteRequest.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class RequestFactory{
    public:
        std::map<std::string, std::string> requestconfig;
        RequestFactory();
        RequestFactory(configuration);
        RequestFactory(std::string, configuration);
        ~RequestFactory();
        static request * factoryRequest(std::string, configuration);
};
