/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:51:22 by xbasabe-          #+#    #+#             */
/*   Updated: 2024/11/06 18:52:45 by xbasabe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorHandler.hpp"

errorHandler::errorHandler(std::string code, int sock) : error_code(code), socket(sock){

}

errorHandler::errorHandler(){

}

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

errorHandler::~errorHandler(){

}

std::string errorHandler::errorHead(std::string error){
    std::map<const char*, int> errors;
    errors["404"] = 0;
    errors["403"] = 1;
    //include more cases

    int i = 0;

    for (std::map<const char*, int>::iterator it = errors.begin(); it != errors.end(); it++)
    {
        i = it->second;
        //if (uri.find(it->first) != std::string::npos)
        if(it->first == error)
            break;
    }

    switch(i) {
        case 0:
            std::cout << "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n" << std::endl;
            return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
            break;
        case 1:
            std::cout << "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n" << std::endl;
            return "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n";
            break;
        default:
            // devolver http en todo caso? GET sin uri
            std::cout << "NOT WORKING CASE" << std::endl;
            //return "default";
    }
    return "default";
}

std::string errorHandler::errorPage(std::string error){
    std::map<const char*, int> errors;
    errors["404"] = 0;
    errors["403"] = 1;
    //include more cases

    int i = 0;

    for (std::map<const char*, int>::iterator it = errors.begin(); it != errors.end(); it++)
    {
        i = it->second;
        //if (uri.find(it->first) != std::string::npos)
        if(it->first == error)
            break;
    }

    switch(i) {
        case 0:
            std::cout << "HTTP/1.1 404 Not Found" << std::endl;
            return "404.html";
            break;
        case 1:
            std::cout << "HTTP/1.1 403 Forbidden" << std::endl;
            return "403.html";
            break;
        default:
            // devolver http en todo caso? GET sin uri
            std::cout << "NOT WORKING CASE" << std::endl;
            //return "default";
    }
    return "default";
}

std::string errorHandler::errorReply()
{
    std::ifstream resource(errorPage(error_code));
    std::string response = errorHead(error_code);
    write(socket, response.c_str(), response.size());
    while (std::getline(resource, response)) {
        write(socket, response.c_str(), response.size());
    }
    resource.close();
}
