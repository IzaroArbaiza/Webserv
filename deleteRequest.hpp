#pragma once
#include "request.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class deleteRequest : public request{
    private:
        std::string location;
        std::string uri_type;
        std::string vbles;
        std::string file_name;
        std::string resource;

    public:
        deleteRequest (std::string);
        deleteRequest ();
        deleteRequest (deleteRequest&);
        deleteRequest& operator=(deleteRequest&);
        void fill (std::string); //GET y POST diferente funcion fill para coger datos de la uri o del body
        ~deleteRequest();
        void uri_extention();
        void fillLocation();
        void fillFile();
        void deleteParse();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
};

