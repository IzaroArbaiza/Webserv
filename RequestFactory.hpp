#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <functional>
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
        RequestFactory(std::map<std::string, std::string>);
        RequestFactory(std::string, std::map<std::string, std::string>);
        ~RequestFactory();
        static request * factoryRequest(std::string);
};
