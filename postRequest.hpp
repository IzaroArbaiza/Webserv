#pragma once
#include "request.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class postRequest : public request{
    private:
        std::string location;
        std::string uri_type;
        std::vector<std::string> vbles;
        std::string exec_vbles;
        std::string file_name;
        

    public:
        std::string request_body;
        std::string boundary;
        std::string file_content;
        postRequest (std::string);
        postRequest ();
        postRequest (postRequest&);
        postRequest& operator=(postRequest&);
        void fill(std::string); //GET y POST diferente funcion fill para coger datos de la uri o del body
        void initPost(std::string);
        void fillVbles();
        void extractBody();
        void parseBody();
        void parse_resource();
        void parseBodyUpload();
        void fillLocation(std::string);
        ~postRequest();
        void uri_extention();
        std::string getVbles();
        std::string getLocation();
        std::string getFileName();
        std::string actionDetector();
        void fillFilename();
        std::string getFileContent();
        //std::string getUri();
        //std::string getBody();
        //std::string uriDetector();
        //std::string getRequestLine();
};

