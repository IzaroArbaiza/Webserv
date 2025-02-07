#pragma once

#include "struct.hpp"
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "response.hpp"


class uploadHandler{
    private:
        int _sock;
        std::string file_name;

    public:
        uploadHandler();
        uploadHandler(int);
        void responde(request*, configuration conf);
        std::string fileName(request * entry);
        //std::string vblesPost(request * entry);
        std::string fileContent(request * entry);
        void setFileName(std::string);
        std::string getFileName();
};
