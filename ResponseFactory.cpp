#include "ResponseFactory.hpp"

ResponseFactory::ResponseFactory(){}

ResponseFactory::ResponseFactory(std::string buffer, std::map<std::string, std::string> config){
    factoryResponse(buffer, config);
}

ResponseFactory::~ResponseFactory(){}

response * ResponseFactory::factoryResponse(std::string readed, std::map<std::string, std::string> config){ //meter aqui rawRequest en vz de en el constructor?
    std::string tmpRequestLine;
    std::string tmpMethod;
    tmpRequestLine = readed.substr(0, tmpRequestLine.find(CRLF)); //first line of the message is the Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    tmpMethod = tmpRequestLine.substr(0, tmpRequestLine.find(" "));
    if (tmpMethod == "GET") //type = html; cgi, error... a que respondemos y no el tipo de metodo de petición  ¿que tipos de respuestas tenemos?
        return new getRequest();
    if (tmpMethod == "POST")
        return new postRequest();
    //mas casos? favicoon, cgi, delete, upload...?
}