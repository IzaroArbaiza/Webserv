/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:46 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:47 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/deleteRequest.hpp"

deleteRequest::deleteRequest(std::string message, configuration config): request(message){
    init();
    (void) config;
}

deleteRequest::deleteRequest(){parseURL();}

deleteRequest::deleteRequest(deleteRequest& cp){
    *this = cp;
}

deleteRequest & deleteRequest::operator=(deleteRequest& cp){
    (void)cp;
    return *this;
}

void deleteRequest::init(){
    std::vector<std::string> splited;
    request_line = rawRequest.substr(0, rawRequest.find(CRLF));
    splited = vector_split(request_line, " ");
    method =  splited[0];
    uri = splited[1];
    if (splited[1].length() > 1){
        path = splited[1];
        path = path.substr(0,path.find_last_of("/"));
        file_name = splited[1].substr(splited[1].find_last_of("/") + 1);
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
}

void deleteRequest::fillFile(){
    file_name = getUri();
    file_name = file_name.substr(file_name.find("file=") + 5);
}

deleteRequest::~deleteRequest(){}
