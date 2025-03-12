/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:08 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:09 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/postHandler.hpp"

postHandler::postHandler(){}

postHandler::postHandler(int sock) : _sock(sock) {}

void postHandler::responde(request * entry, configuration conf){
    static_cast<postRequest *>(entry)->parse_resource(conf);
    std::string extention = static_cast<postRequest *>(entry)->getFileName();
    
    if (extention.find(".pdf") != std::string::npos){
        response toClient(_sock, "406", conf);
        toClient.reply(conf);
    } else if (static_cast<postRequest *>(entry)->file_content.empty() && static_cast<postRequest *>(entry)->boundary.find("----WebKitFormBoundary") != std::string::npos){
        response toClient(_sock, "409", conf);
        toClient.reply(conf);
    } else if (fileSize(entry) > conf.body_size){
        response toClient(_sock, "413", conf);
        toClient.reply(conf);
    } else if (actionDetector(entry) == "cgi"){
        execute(entry, conf);
    } else if (actionDetector(entry) == "upload"){
        uploadHandler Handler(_sock);
        Handler.responde(entry, conf);
    } else if (actionDetector(entry) == "delete"){
        deleteHandler Handler(_sock);
        Handler.responde(entry, conf);
    } else {
        if (access(static_cast<postRequest *>(entry)->getLocation().c_str(),R_OK) == -1){
            response toClient(_sock, "404", conf);
            toClient.reply(conf);
        } else{
            response toClient(_sock, "200", static_cast<postRequest *>(entry)->getLocation());
            toClient.reply(conf);
        }
    }
}

std::string postHandler::actionDetector(request * entry)
{
    std::string type = entry->header_fields["Content-Type"];
    if (type.find("multipart/form-data") != std::string::npos)
        return "upload";
    else if (type.find("application/x-www-form-urlencoded") != std::string::npos)
        return "cgi";
    return (0);
}

void postHandler::execute(request * entry, configuration conf){
    std::string check = static_cast<getRequest *>(entry)->getResource();
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
    } else {
        if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR)) {
            cgiHandler Handler(_sock);
            Handler.responde(entry, conf);
        } else {
            response toClient(_sock, "403", conf);
            toClient.reply(conf);
        }
    }
}

std::string postHandler::file_type(std::string route){
    struct stat st;
    route = getcwd(NULL,0) + route;
    if (stat(route.c_str(), &st) == -1) {
        return "error";
    } else {
        if (S_ISDIR(st.st_mode)) {
            return "folder";
        } else{
            if (S_ISREG(st.st_mode)){
                if (st.st_mode & S_IXUSR)
                    return "executable";
                else
                    return "file";
            }
        }
    }
    return (0);
}

size_t postHandler::fileSize(request *entry){
    std::string raw = static_cast<postRequest *>(entry)->file_content;
    return raw.size();
}

