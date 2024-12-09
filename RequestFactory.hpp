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
        RequestFactory();
        RequestFactory(std::string);
        ~RequestFactory();
        static request * factoryRequest(std::string);
};
