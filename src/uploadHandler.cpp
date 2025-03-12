/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uploadHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:21:35 by xbasabe-          #+#    #+#             */
/*   Updated: 2025/03/12 18:24:40 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/uploadHandler.hpp"

uploadHandler::uploadHandler(){}

uploadHandler::uploadHandler(int sock) : _sock(sock) {}

void uploadHandler::responde(request * entry, configuration conf){
    std::string downi=  conf.root + "/uploads/"+ fileName(entry);
    std::ofstream ofs(downi.c_str());
    /*if(ofs.fail()){
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
        exit(1);
    }*/
    ofs << fileContent(entry);
    ofs.close();
    response toClient(_sock, "200", "/pages/uploaded.html");
    toClient.reply("Uploaded file: " + fileName(entry), conf);
}

std::string uploadHandler::fileContent(request * entry){
    std::string raw = static_cast<postRequest *>(entry)->file_content;
    std::string parsed_content;
    parsed_content = raw.substr(raw.find("\r\n\r\n") + 4);
    return parsed_content;
}

void uploadHandler::setFileName(std::string str){
    file_name = str;
}

std::string uploadHandler::getFileName(){
    return file_name;
}

std::string uploadHandler::fileName(request * entry){
    size_t from;
    size_t end;
    std::string file_name;

    from = static_cast<postRequest *>(entry)->request_body.find("filename=\"") + 10;
    end = static_cast<postRequest *>(entry)->request_body.find("\"", from);
    file_name = static_cast<postRequest *>(entry)->request_body.substr( from, end - from);
    return file_name;
}
