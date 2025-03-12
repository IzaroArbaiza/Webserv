/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:51:22 by xbasabe-          #+#    #+#             */
/*   Updated: 2025/03/12 18:23:51 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/errorHandler.hpp"

errorHandler::errorHandler(std::string code, int sock) : error_code(code), socket(sock){}

errorHandler::errorHandler(){}

errorHandler::errorHandler(errorHandler& cp){
    *this = cp;
}

errorHandler & errorHandler::operator=(errorHandler& cp){
    this->_header = cp._header;
    return *this;
}

std::string errorHandler::getErrorCode(){
    return error_code;
}

std::string errorHandler::getUri(){
    return uri;
}

std::string errorHandler::getAnswer(){
    return answer;
}

errorHandler::~errorHandler(){}

std::string errorHandler::errorHead(std::string error){
    std::map<const char*, int> errors;
    errors["404"] = 0;
    errors["403"] = 1;
    int i = 0;

    for (std::map<const char*, int>::iterator it = errors.begin(); it != errors.end(); it++) {
        i = it->second;
        if(it->first == error)
            break;
    }
    switch(i) {
        case 0:
            return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
            break;
        case 1:
            return "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n";
            break;
        default:
            std::cout << "NOT WORKING CASE" << std::endl;
    }
    return "default";
}

std::string errorHandler::errorPage(std::string error){
    std::map<const char*, int> errors;
    errors["404"] = 0;
    errors["403"] = 1;
    int i = 0;

    for (std::map<const char*, int>::iterator it = errors.begin(); it != errors.end(); it++) {
        i = it->second;
        if(it->first == error)
            break;
    }
    switch(i) {
        case 0:
            return "404.html";
            break;
        case 1:
            return "403.html";
            break;
        default:
            std::cout << "NOT WORKING CASE" << std::endl;
    }
    return "default";
}
