#include "RequestFactory.hpp"

RequestFactory::RequestFactory(std::map<std::string, std::string> config){
    requestconfig = config;
}

RequestFactory::RequestFactory(){
}

RequestFactory::RequestFactory(std::string buffer, std::map<std::string, std::string> config){
    requestconfig = config;
    factoryRequest(buffer);
}

RequestFactory::~RequestFactory(){}

request * RequestFactory::factoryRequest(std::string readed){ //meter aqui rawRequest en vz de en el constructor?
    std::string tmpRequestLine;
    std::string tmpMethod;
    tmpRequestLine = readed.substr(0, tmpRequestLine.find(CRLF)); //first line of the message is the Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    tmpMethod = tmpRequestLine.substr(0, tmpRequestLine.find(" "));
    if (tmpMethod == "GET")
        return new getRequest(readed);
    if (tmpMethod == "POST")
        return new postRequest(readed);
    if (tmpMethod == "DELETE")
      return new deleteRequest(readed);
}