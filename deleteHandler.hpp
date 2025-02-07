#pragma once

#include "struct.hpp"
#include "request.hpp"
#include "getRequest.hpp"

#define CRLF "\r\n"

class deleteHandler{
    private:
        int _sock;

    public:
    deleteHandler();
    deleteHandler(int);
    void responde(request*, configuration conf);
};
