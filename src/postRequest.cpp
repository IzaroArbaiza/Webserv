/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:12 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:13 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/postRequest.hpp"

postRequest::postRequest(std::string str, configuration config): request(str) {
    init();
    parse_resource(config);
}

postRequest::postRequest(){}

postRequest::postRequest(postRequest& cp){
    *this = cp;
}

postRequest &postRequest::operator=(postRequest& cp){
    (void)cp;
    return *this;
}

void postRequest::init(){
    boundary = header_fields["Content-Type"].substr(header_fields["Content-Type"].find("boundary=") + 9, header_fields["Content-Type"].find(CRLF));
    extractBody();
    if (actionDetector() == "cgi"){
        exec_vbles = request_body.substr(request_body.find_first_of("=") + 1);
        fillFilename();
    }
    if (actionDetector() == "upload"){
        std::string boundary = header_fields["Content-Type"].substr(header_fields["Content-Type"].find("boundary=") + 9, header_fields["Content-Type"].find(CRLF));
        std::vector<std::string> body_fields;
        fillFilename();
        if (!request_body.empty() && request_body.find(boundary) != std::string::npos){
            body_fields = vector_split(request_body, "--" + boundary);
            file_content = body_fields[1];
        }
    }
}

void postRequest::extractBody(){
    size_t pos = rawRequest.find("\r\n\r\n");
    request_body = rawRequest.substr(pos);
}

void postRequest::fillVbles(){}

void postRequest::parse_resource(configuration config){
    resource = config.activeDirectory;
    if(!path.empty())
        resource = resource + path;
    if (!getFileName().empty())
        resource = resource + "/" + getFileName();
    config.activeDirectory = resource.substr(resource.find(config.activeDirectory) + resource.size());
}

void postRequest::parseBodyUpload(){}

std::string postRequest::getLocation(){
    return location;
}

void postRequest::fillLocation(std::string str){
    size_t from;
    size_t end;

    from = str.find("filename=\"") + 10;
    end = str.find("\"", from);
    file_name = str.substr( from, end - from);
    location = header_fields["Referer"].append(file_name);
}

void postRequest::fillFilename() {
    file_name = request_body.substr(request_body.find_first_of("=") + 1, request_body.find_first_of("&") - request_body.find_first_of("=") - 1);
    size_t pos = file_name.find(CRLF);
    while (pos != std::string::npos) {
        file_name.erase(pos, 2);
        pos = file_name.find(CRLF);
    }
}

void postRequest::uri_extention(){
    uri_type = uri.substr(uri.find(".") + 1);
}

postRequest::~postRequest(){}

std::string postRequest::getVbles(){
    return exec_vbles;
}

std::string postRequest::getFileName(){
    return file_name;
}

std::string postRequest::getFileContent(){
    return file_content;
}

std::string postRequest::actionDetector()
{
    std::string type = header_fields["Content-Type"];
    
    if (type.find("multipart/form-data") != std::string::npos)
        return "upload";
    else if (type.find("application/x-www-form-urlencoded") != std::string::npos)
        return "cgi";
    return (0);
}

void postRequest::parseUri(configuration & conf){
    std::string tmp_uri = uri;
    if (tmp_uri.find_first_of("?") != std::string::npos)
        tmp_uri = tmp_uri.substr(0, tmp_uri.find_first_of("?")) + "/" + tmp_uri.substr(tmp_uri.find_first_of("?") + 1);
    if (uri.find("cgi") != std::string::npos){
        questionUri(conf);
    } else {
        path = tmp_uri;
        noQuestionUri(conf);
    }
}

void postRequest::questionUri(configuration & conf){
    std::string folder;

    folder = path;
    if (folder.find(".") != std::string::npos){
        file_name = folder.substr(folder.find_last_of("/"));
        folder = folder.substr(0, folder.find_last_of("/"));
    }
    if (file_name.find("/") != std::string::npos)
        file_name = file_name.substr(file_name.find("/") + 1);
    if (file_name.find("..") != std::string::npos || !file_name.empty())
        conf.activeDirectory = conf.activeDirectory + "/" + file_name;
    if (file_name == ".."){
        conf.activeDirectory = conf.activeDirectory.substr(0, conf.activeDirectory.find_last_of("/"));
        conf.activeDirectory = conf.activeDirectory.substr(0, conf.activeDirectory.find_last_of("/"));
    }
    resource = conf.activeDirectory;
    std::string executer;
    for (size_t i = 0; i < conf.locations.size(); i++){
        if (conf.locations[i].location_name == "/cgi-bin"){
            executer = conf.locations[i].location_root;
            break;
        }
    }
    resource =  executer + "/cgi-bin" + "/" + file_name;
}

void postRequest::noQuestionUri(configuration & conf){
    std::string folder;
 
    if (path.find(conf.root) != std::string::npos)
        path = path.substr(path.find(conf.root) + conf.root.size());
    if (path.find(conf.init_root ) != std::string::npos)
        path = path.substr(path.find(conf.root) + conf.root.size());
    if (path.find("//") != std::string::npos )
        path = path.substr(path.find_first_of("/") + 1);
    folder = path;
    if (folder.find(".") != std::string::npos){
        file_name = folder.substr(folder.find_last_of("/"));
        folder = folder.substr(0, folder.find_last_of("/"));
    }
    if (file_name.find("/") != std::string::npos)
        file_name = file_name.substr(file_name.find("/") + 1);
    std::string base = conf.root;
    if (!root_is_location(conf))
        base = conf.init_root;
    if (path.compare(conf.activeDirectory))
        conf.activeDirectory = path;
    if (file_name == ".."){
        conf.activeDirectory = conf.activeDirectory.substr(0, conf.activeDirectory.find_last_of("/"));
        conf.activeDirectory = conf.activeDirectory.substr(0, conf.activeDirectory.find_last_of("/"));
        }
    if (file_name == ".")
        conf.activeDirectory = conf.activeDirectory.substr(0, conf.activeDirectory.find_last_of("/"));
    if (folder.empty() && file_name.empty())
        conf.activeDirectory = conf.root;
    resource = base + conf.activeDirectory;
}

bool postRequest::root_is_location(configuration & conf){
    for(int i = 0; i < conf.iter; i++){
        if ( path == conf.locations[i].location_name || (path.empty() && conf.locations[i].location_name =="/"))
            return true;
    }
    return false;
}
