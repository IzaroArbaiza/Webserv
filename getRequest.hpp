#pragma once
#include "request.hpp"


#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class getRequest : public request{
    private:
        std::string location;
        std::string uri_type;
        std::string vbles;
        std::string file_name;

    public:
        bool redir_index; 
        getRequest (std::string, configuration);
        getRequest ();
        getRequest (getRequest&);
        getRequest& operator=(getRequest&);
        void init();
        ~getRequest();
        void uri_extention();
        void parse_location();
        void parse_file();
        void parse_resource(configuration);
        std::string extractDirectoryPath(const std::string& fullPath);
        bool check_index();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
        void parseVbles();
};
