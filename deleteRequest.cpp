#include "deleteRequest.hpp"

deleteRequest::deleteRequest(std::string message, configuration config): request(message){
    init();
    (void) config;
    std::cout << "deleteRequest created" << std::endl;
}

deleteRequest::deleteRequest(){ std::cout << "deleteRequest created" << std::endl; parseURL();}

deleteRequest::deleteRequest(deleteRequest& cp){
    *this = cp;
}

deleteRequest & deleteRequest::operator=(deleteRequest& cp){
    (void)cp;
    //this->_header = cp._header; //actualizar los campos que se copian
    return *this;
}

void deleteRequest::init(){ //reescribir usando el como ejemplo el curl?
    std::vector<std::string> splited;
    //std::cout << "parseamos el texto: " << rawRequest<< std::endl;
    request_line = rawRequest.substr(0, rawRequest.find(CRLF));
    std::cout << "delete request line" << request_line << std::endl;
    splited = split(request_line, " ");
    method =  splited[0];
    uri = splited[1];
    std::cout << "delete request uri: " << uri << std::endl;
    if (splited[1].length() > 1){ //withput query the resut of splited[1] is "/"
        path = splited[1]; //path + query  /carpeta?vbles
        path = path.substr(0,path.find_last_of("/"));
        std::cout << "deleteParse path " << path << std::endl;
        file_name = splited[1].substr(splited[1].find_last_of("/") + 1);
        std::cout << "deleteParse file " << file_name << std::endl; // no tenemos en cuenta que podria venir #ancla para ir a una zona de la pagina
    }
    scheme = splited[2];
}

std::string deleteRequest::getLocation(){ return location; }

std::string deleteRequest::getUriType(){ return uri_type; }

std::string deleteRequest::getVbles(){ return vbles; }

std::string deleteRequest::getFileName(){ return file_name; }

std::string deleteRequest::getResource(){ return resource; }

void deleteRequest::uri_extention(){
    uri_type = uri.substr(uri.find(".") + 1);
    std::cout << "URI extention: " << uri_type << std::endl;
}
/* 
void deleteRequest::fillLocation(){
    //std::string resource;
    if (getUri() == ""){
        location = "index.html";
        //resource = _header["Host"];
        resource.append(location);
    }
    else{
        location = getUri();
        if (uri.find("?") != std::string::npos){ //existe un '?' en la uri
            location = location.substr(1,location.find('?') - 1);
            resource = header_fields["Referer"];
            resource.append(location);
            resource.append("/" + file_name);
        }
        else
            location = location.substr(location.find("/") + 1);
    }
} */

void deleteRequest::fillFile(){
    file_name = getUri();
    file_name = file_name.substr(file_name.find("file=") + 5);
    std::cout << "getRequest fillFile. file: " << file_name << std::endl;
}

deleteRequest::~deleteRequest(){

}
