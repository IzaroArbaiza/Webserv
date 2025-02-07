#pragma once

#include "response.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request{
    protected:
        std::string request_line; //esta seria la URL
        std::string method;
        std::string uri;
        std::string version;
        std::string URL;
        std::string scheme; //=protocolo
        std::string authority; //dominio:puerto ¿es igual a domain?
        std::string domain;
        std::string path; // uri . ruta, al elemento o pagina concreta
        std::string query_string; //desde ? pares key=value
        std::string extention; //a que lleva la direccion: carpeta (sin extensión), pagina (.html) o ejecutable (.py, .out)
        std::string resource;

    public:
        std::map<std::string, std::string> header_fields;
        std::string rawRequest;
        request (std::string);
        request ();
        request (request&);
        request& operator=(request&);
        virtual void init() = 0; //no instanciable
        virtual ~request();
        std::string getMethod();
        std::string getUri();
        std::string getRequestLine();
        void parseURL();
        void parseHeader();
        void unfolding(std::string &str);
        void checkExtention();
        bool isFolder(std::string);
        //getters
        std::string getScheme();
        std::string getPath();
        std::string getExtention();
        std::string getQueryString();
        std::string getDomain();
        std::string getResource();
        //setters
        void setResource(std::string res);
};
std::vector<std::string> split(const std::string& s, const std::string& delimiter);
