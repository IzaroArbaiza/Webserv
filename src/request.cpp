/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:15 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:16 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/request.hpp"

std::string trim(std::string &s);

request::request(std::string received) : rawRequest(received){
    parseURL();
    parseHeader();
}

request::request(){}

request::request(request& cp){
    *this = cp;
}

request & request::operator=(request& cp){
    this->header_fields = cp.header_fields;
    return *this;
}

void request::init(){}

std::string request::getMethod(){ return method; }

std::string request::getUri(){ return uri; }

std::string request::getRequestLine(){ return request_line; }

std::string request::getScheme(){ return scheme; }

std::string request::getPath(){ if (!path.empty())return path; else return "/"; }

std::string request::getExtention(){ return extention; }

std::string request::getQueryString(){ return query_string; }

std::string request::getDomain(){ return domain; }

std::string request::getResource(){ return resource; }

void request::setResource(std::string res){ resource = res; }
void request::setUri(std::string newUri){ uri= newUri; }

request::~request(){}

void request::parseURL(){
    std::vector<std::string> splited;
    request_line = rawRequest.substr(0, rawRequest.find(CRLF));
    splited = vector_split(request_line, " ");
    if (splited.size() != 3)
        std::cout << "error: incorrect request line" << std::endl;
    splited[0] = trim(splited[0]);
    splited[1] = trim(splited[1]);
    splited[2] = trim(splited[2]);
    method =  splited[0];
    uri = splited[1];
    if (splited[1].length() > 1){
        resource = uri.substr(uri.find_last_of("/"));
        path = uri;
        path = path.substr(0, path.find("?"));
        if (splited[1].find("?") != std::string::npos){
            query_string = splited[1].substr(splited[1].find("?") + 1);
        }
    }
    scheme = splited[2];
    if (scheme != "HTTP/1.1")
        std::cout << "Error: 505 HTTP Version Not Supported" << scheme << std::endl;
}

void request::parseHeader(){
    std::vector<std::string> lines;
    std::string request_header;

    request_header = rawRequest.substr(rawRequest.find(CRLF), rawRequest.find("\r\n\r\n") - rawRequest.find(CRLF));
    request_header = trim(request_header);
    lines = vector_split(request_header, CRLF);
    for (int i = 1; i < (int)lines.size() - 2; i++){
        if (lines[i] == "\r\n\r\n"){
            break ;
        }
        header_fields[lines[i].substr(0,lines[i].find_first_of(":"))] = lines[i].substr(lines[i].find_first_of(":") + 2);
    }
    domain = header_fields["Host"];
}

std::vector<std::string> vector_split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0U;
    size_t end = s.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start, end - start));
    return tokens;
}

std::string trim(std::string &s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");

    if (start == std::string::npos)
        s.clear();
    else
        s = s.substr(start, end - start + 1);
    return s;
}

std::string request::upFolder(std::string line){
    std::string folder = line;
    if (folder.find("..") != std::string::npos){
        folder = folder.substr(0, folder.find("..") - 1);
        folder = folder.substr(0, folder.find("/") + 1);
        return folder;
    }
    return line;
}

