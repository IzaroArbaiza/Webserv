/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:23 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:24 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/response.hpp"

response::response(){}

response::response(int sock) : socket(sock){}

response::response(int sock, std::string status) : socket(sock), code(status){}

response::response(int sock, std::string status, std::string page) : socket(sock), code(status), location(page){}

response::response(int sock, std::string status, configuration conf) : socket(sock), code(status){
    (void) conf;
}

void response::fill_header(){
    std::string content_type;
    std::string content_length = "Content-Length:";
    if (location.substr(location.find(".")) == ".html")
        content_type = "Content-type: text/html";
    if (location.substr(location.find(".")) == ".ico")
        content_type = "Content-Type: image/x-icon";
    if (location.substr(location.find(".")) == ".jpg")
        content_type = "Content-Type: image/jpg";
    if (location.substr(location.find(".")) == ".jpeg")
        content_type = "Content-Type: image/jpeg";
    if (location.substr(location.find(".")) == ".png")
        content_type = "Content-Type: image/png";
    std::string conection = "Connection:Closed";
    header.append(content_type);
}

void response::redir_header(std::string location){
    std::string content_type = "Content-type: text/html\r\n";
    header.append(content_type);
    std::string new_location = "Location: ./";
    new_location.append(location);
    header.append(new_location);
}

void response::fill_body_answer(){}

void response::fill_body(std::string code, configuration conf){
    locationPage(code, conf);
}

std::string response::convertOutput(){
    std::string output;

    output.append(status_line);
    output.append("\r\n");
    output.append(header);
    output.append("\r\n\r\n");
    output.append(body);
    return output;
}

void response::fill_status_line(std::string code){
    std::map<const char*, int> codes;
    codes["200"] = 0;
    codes["404"] = 1;
    codes["403"] = 2;
    codes["500"] = 3;
    codes["505"] = 4;
    codes["418"] = 5;
    codes["204"] = 6;
    codes["308"] = 7;
    codes["405"] = 8;
    codes["409"] = 9;
    codes["406"] = 10;
    
    int i = 0;
    for (std::map<const char*, int>::iterator it = codes.begin(); it != codes.end(); it++)
    {
        i = it->second;
        if(it->first == code)
            break;
    }
    switch(i) {
        case 0:
            status_line.append("HTTP/1.1 200 OK");
            break;
        case 1:
            status_line.append("HTTP/1.1 404 Not Found");
            break;
        case 2:
            status_line.append("HTTP/1.1 403 Forbidden");
            break;
        case 3:
            status_line.append("HTTP/1.1 500 internal server error");
            break;
        case 4:
            status_line.append("HTTP1.1 505 HTTP Version Not Supported");
            break;
        case 5:
            status_line.append("HTTP1.1 418 I'm a teapot. This is not a folder");
            break;
        case 6:
            status_line.append("HTTP/1.1 204 No content");
            break;
        case 7:
            status_line.append("HTTP/1.1 308 Permanent Redirect");
            break;
        case 8:
            status_line.append("HTTP/1.1 405 Method Not Allowed");
            break;
        case 9:
            status_line.append("HTTP/1.1 409 Conflict");
            break;
        case 10:    
            status_line.append("HTTP/1.1 406 Not Acceptable");
            break;
        default:
            std::cout << "NOT WORKING CASE" << std::endl;
    }
}

void response::locationPage(std::string code, configuration conf) {
    std::map<const char*, int> codes;
    codes["200"] = 0;
    codes["404"] = 1;
    codes["403"] = 2;
    codes["500"] = 3;
    codes["418"] = 4;
    codes["204"] = 5;
    codes["405"] = 6;
    codes["413"] = 7;
    codes["308"] = 8;
    codes["505"] = 9;
    codes["409"] = 10;
    codes["406"] = 11;

    int i = 0;
    for (std::map<const char*, int>::iterator it = codes.begin(); it != codes.end(); it++)
    {
        i = it->second;
        if(it->first == code)
            break;
    }
    switch(i) {
        case 0:
            readPage(location);
            break;
        case 1:
            readPage(conf.errors[404]);
            break;
        case 2:
            readPage(conf.errors[403]);
            break;
        case 3:
            readPage(conf.errors[500]);
            break;
        case 4:
            readPage(conf.errors[418]);
            break;
        case 5:
            readPage(conf.errors[204]);
            break;
        case 6:
            readPage(conf.errors[405]);
            break;
        case 7:
            readPage(conf.errors[413]);
            break;
        case 8:
            readPage(conf.errors[413]);
            break;
        case 9:
            readPage(conf.errors[505]);
            break;
        case 10:
            readPage(conf.errors[409]);
            break;
        case 11:
            readPage(conf.errors[406]);
            break;
        default:
            std::cout << "NOT WORKING CASE" << std::endl;
    }
}

void response::readPage(std::string location){
    std::ifstream resource(location.c_str());
    std::string buffer;
    while (std::getline(resource, buffer)) {
        body.append(buffer);
    }
    resource.close();
}

void response::reply(std::string to_body, configuration conf){
    fill_status_line(code);
    fill_header();
    fill_body(code, conf);
    std::string response = convertOutput();
    if (to_body != "")
        response.append(to_body);
    write(this->socket, response.c_str(), response.size());
}

void response::reply(configuration conf){
    fill_status_line(code);
    if (code == "200")
        fill_header();
    fill_body(code, conf);
    std::string response = convertOutput();
    write(this->socket, response.c_str(), response.size());
}

void response::redirect(std::string location, configuration conf){
    fill_status_line(code);
    redir_header(location);
    fill_body(code, conf);
    std::string response = convertOutput();
    write(this->socket, response.c_str(), response.size());
}

