/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:56 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:57 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/getRequest.hpp"

std::string normalizeSlashes(const std::string& input);

getRequest::getRequest(std::string message, configuration config): request(message){
    init();
    (void) config;
}


getRequest::getRequest(){parseURL();}

getRequest::getRequest(getRequest& cp){
    *this = cp;
}

getRequest & getRequest::operator=(getRequest& cp){
    (void)cp;
    return *this;
}

void getRequest::init(){
    parse_file();
    parse_location();
    parseVbles();
}

void getRequest::parseVbles(){
    vbles = query_string.substr(query_string.find_first_of("&") + 1);
}

std::string getRequest::getLocation(){ return location; }

std::string getRequest::getUriType(){ return uri_type; }

std::string getRequest::getVbles(){ return vbles; }

std::string getRequest::getFileName(){ return file_name; }

std::string getRequest::getResource(){ return resource; }

void getRequest::uri_extention(){
    uri_type = uri.substr(uri.find(".") + 1);
}

void getRequest::parse_location(){
    location = path;
    if (query_string != ""){
        location = location.substr(1,location.find('?') - 1);
    }
    else
        location = location.substr(location.find("/") + 1);
}

void getRequest::parse_file(){
    file_name = getQueryString();
    if (file_name.find("file=") != std::string::npos)
        file_name = file_name.substr(file_name.find("file=") + 5);
    else 
        file_name = file_name.substr(file_name.find("?") + 1, file_name.find("&") - file_name.find("?") - 1);
}

void getRequest::parse_resource(configuration & config){
    resource = config.root;
    if(!path.empty())
        resource = resource + path;
    if (!getFileName().empty())
        resource = resource + "/" + getFileName();
}

bool getRequest::check_index(){
    DIR *dir;
    struct dirent *ent;
    std::string path = getResource();
    dir = opendir(path.c_str());
    bool is = false; 
    
    struct stat st;
    if (stat(path.c_str(), &st) == -1) {
        is = false;
    }
    if (S_ISDIR(st.st_mode))
        is = true;

    if (!is)
        return false;
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, "index.html") == 0){
            closedir(dir);
            return true;
        }
    }
    closedir(dir);
    return false;
}

getRequest::~getRequest(){}

void getRequest::parseUri(configuration & conf){
    std::string tmp_uri = uri;
    if (tmp_uri.find_first_of("?") != std::string::npos){
        tmp_uri = tmp_uri.substr(0, tmp_uri.find_first_of("?")) + "/" + tmp_uri.substr(tmp_uri.find_first_of("?") + 1);
    }
    if (uri.find("cgi") != std::string::npos){
        questionUri(conf);
    } else{
        path = tmp_uri;
        noQuestionUri(conf);
    }
}

void getRequest::questionUri(configuration & conf){
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
    if (executer == "./")
        executer= conf.init_root;
    resource =  executer + "/cgi-bin" + "/" + file_name;
    resource = normalizeSlashes(resource);
}

void getRequest::noQuestionUri(configuration & conf){
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
    if (base == "./")
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
    resource = normalizeSlashes(resource);
}

bool getRequest::root_is_location(configuration & conf){
    for(int i = 0; i < conf.iter; i++){
        if ( path == conf.locations[i].location_name || (path.empty() && conf.locations[i].location_name =="/"))
            return true;
    }
    return false;
}

std::string normalizeSlashes(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    
    size_t i = 0;
    while (i < input.size()) {
        result += input[i];
        if (input[i] == '/' && i + 1 < input.size() && input[i + 1] == '/') {
            i += 2;
            continue;
        }
        i++;
    }
     if (!result.empty() && result[result.size() - 1] == '/') {
        result.erase(result.size() - 1);
    }
    return result;
}
