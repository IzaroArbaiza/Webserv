/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:52 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:37:42 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/getHandler.hpp"

getHandler::getHandler(){}

getHandler::getHandler(int sock) : _sock(sock) {
}

void getHandler::responde(request * entry, configuration & conf){
    static_cast<getRequest *>(entry)->parseUri(conf);
    std::string resource_type = accesable(entry);

    if (resource_type == "error_403"){
        response toClient(_sock, "403", conf);
        toClient.reply(conf);
    } else if (resource_type == "error_404"){
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
    } else if (resource_type == "no"){
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
    } else if (resource_type == "folder"){
        if (conf.autoindex){   
            autoindex(entry, conf);

        }    
        else{
            if (!index_in_folder(entry, conf)){
                response toClient(_sock, "404", conf);
                toClient.reply(conf);
            }
        }
    } else if (isExecutable(entry, conf)){
        cgiHandler Handler(_sock);
        Handler.responde(entry, conf);
    } else{
        if (access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1){
            response toClient(_sock, "403", conf);
            toClient.reply(conf);
        } else{
            response toClient(_sock, "200", static_cast<getRequest *>(entry)->getResource());
            toClient.reply(conf);
        }
    }
}

bool getHandler::isExecutable(request * entry, configuration conf){
    std::string check = static_cast<getRequest *>(entry)->getResource();
    (void) conf;
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        return false;
    } else if ((sb.st_mode & S_IXUSR) == S_IXUSR && !S_ISDIR(sb.st_mode)){
        return true;
    } else {
        return false;
    }
}

std::string getHandler::accesable(request * entry){
    std::string check = entry->getResource();
    struct stat sb;
    
    if (stat(check.c_str(), &sb) == -1) {
        return "error_404";
    }
    if (S_ISDIR(sb.st_mode)){
        return "folder";
    } else if (S_ISREG(sb.st_mode) && check.find("html") != std::string::npos){
        return "page";
    } else if ((sb.st_mode & S_IXUSR) == S_IXUSR){
        return "exec";
    } else if (S_ISREG(sb.st_mode) && check.find("ico") != std::string::npos){
        return "ico";
    } else if (S_ISREG(sb.st_mode) && check.find("png") != std::string::npos){
        return "png";
    } else if (S_ISREG(sb.st_mode) && check.find("jpg") != std::string::npos){
        return "jpg";
    }
    else if (S_ISREG(sb.st_mode) && check.find("jpeg") != std::string::npos){
        return "jpg";
    }
    else if (S_ISREG(sb.st_mode) && check.find("sh") != std::string::npos){
        return "sh";
    }
    return "error_403";
}

void getHandler::autoindex(request *entry, configuration conf){
    std::string tmp_autoindex;
    DIR *dir;
    struct dirent *ent;
    std::string path = static_cast<getRequest*>(entry)->getResource();
    
    dir = opendir(path.c_str());
    if (dir == NULL) {
        std::cout << "Error opening the directory" << std::endl;
        closedir(dir);
    } else {
        tmp_autoindex = "<html><code>";
        while ((ent = readdir(dir)) != NULL) {
            tmp_autoindex = tmp_autoindex + "<ul>" + "<a href=" + static_cast<getRequest*>(entry)->getResource() + "?" + ent->d_name + ">"  + ent->d_name + "</a>" + "</ul>";
        }
        tmp_autoindex = tmp_autoindex + "</code></html>";
    }
    closedir(dir);
    response toClient(_sock, "200", conf.root + "/pages/content.html");
    toClient.reply(tmp_autoindex, conf);
}

bool getHandler::index_in_folder(request *entry, configuration conf){
    DIR *dir;
    struct dirent *ent;
    std::string path = static_cast<getRequest*>(entry)->getResource();
    std::ofstream tmp_autoindex("list", std::ios::out);

    dir = opendir(path.c_str());
    if (dir == NULL) {
        std::cout << "Error opening the directory" << std::endl;
        closedir(dir);
    } else {
        tmp_autoindex << "<html><code>";
        while ((ent = readdir(dir)) != NULL) {
            std::string file = ent->d_name;
            std::string parsed_index = "index.html";
            std::string resor = static_cast<getRequest*>(entry)->getResource();
            if (!conf.index.empty())
                parsed_index = conf.index;
            if (!file.compare(parsed_index) ){
                if (resor.find("//") != std::string::npos ){
                    resor = resor.substr(resor.find_first_of("/") + 1);
                }
                response toClient(_sock, "200", resor + "/" + ent->d_name );
                toClient.reply(conf);
                closedir(dir);
                tmp_autoindex.close();
                return true;
            }
        }
        tmp_autoindex << "</code></html>";
    }
    closedir(dir);
    tmp_autoindex.close();
    return false;
}
