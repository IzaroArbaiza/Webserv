#pragma once
#include "response.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class response;

class cgiResponse : public response{
    private:
        std::string location;
        std::string uri_type;
        std::vector<std::string> vbles;

    public:
        cgiResponse (std::string);
        cgiResponse ();
        cgiResponse (cgiResponse &);
        cgiResponse& operator=(cgiResponse&);
        //Funciones propias de la clase
        void fillRequest(std::string message);
        void fill(std::string); //GET y POST diferente funcion fill para coger datos de la uri o del body
        void fillLocation();
        void fillVbles();
        void parseBody();

        
        ~cgiResponse();
};