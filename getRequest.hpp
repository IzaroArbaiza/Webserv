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
        //std::string resource; //esta en request. pos y get la parsearan diferente.
        

    public:
        bool redir_index; 
        getRequest (std::string);
        getRequest ();
        getRequest (getRequest&);
        getRequest& operator=(getRequest&);
        void initGet();
        void fill (std::string); //GET y POST diferente funcion fill para coger datos de la uri o del body
        ~getRequest();
        void uri_extention();
        void parse_location();
        void parse_file();
        void parse_resource();
        bool check_index();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
        void parseVbles();
};

