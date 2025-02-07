#include "RequestFactory.hpp"

RequestFactory::RequestFactory(configuration config){
    (void) config;
}

RequestFactory::RequestFactory(){
}

RequestFactory::RequestFactory(std::string buffer, configuration conf){
    factoryRequest(buffer, conf);
}

RequestFactory::~RequestFactory(){}

request * RequestFactory::factoryRequest(std::string readed, configuration conf){ //meter aqui rawRequest en vz de en el constructor?
    std::string tmpRequestLine;
    std::string tmpMethod;
    tmpRequestLine = readed.substr(0, tmpRequestLine.find(CRLF)); //first line of the message is the Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    tmpMethod = tmpRequestLine.substr(0, tmpRequestLine.find(" "));
    if (tmpMethod == "GET")
        return new getRequest(readed, conf);
    if (tmpMethod == "POST")
        return new postRequest(readed, conf);
    if (tmpMethod == "DELETE")
      return new deleteRequest(readed, conf);
    return (0);
}
