#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include "response.hpp"
#include "getRequest.hpp" //cgi response
#include "postRequest.hpp" //html response...

#define CRLF "\r\n"
#define FWL "\r\n "

class ResponseFactory{
    public:
        ResponseFactory();
        ResponseFactory(std::string);
        ~ResponseFactory();
        static response * factoryResponse(std::string);
};
