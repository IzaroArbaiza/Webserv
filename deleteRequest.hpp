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
        //std::string resource; usar la de request

    public:
        deleteRequest (std::string, configuration);
        deleteRequest ();
        deleteRequest (deleteRequest&);
        deleteRequest& operator=(deleteRequest&);
        ~deleteRequest();
        void uri_extention();
        //void fillLocation();
        void fillFile();
        void init();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
};
