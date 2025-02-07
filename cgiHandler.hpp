#pragma once

#include "struct.hpp"
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "response.hpp"


class cgiHandler{
    private:
        int _sock;
        std::string resource;
        //request * entry; copiar al iniciar y ya no necesitamos pasar a funciones
    public:
    cgiHandler();
    cgiHandler(int);
    //cgiHandler(int, std::map<std::string, std::string>);
    //cgiHandler(int, request *);
    void responde(request*, configuration conf);
    void execBinary(std::string file_name, request*, configuration);
    void execPy(std::string file_name, request*, configuration);
    std::string cgiFileName(request * entry);
    std::string vblesMethod(request * entry);
    std::string buildPath(request * entry, configuration);
    std::string envPath(request * entry, configuration);
    void to_location_resource(request * entry, configuration);
};
