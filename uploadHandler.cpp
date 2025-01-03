/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uploadHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:21:35 by xbasabe-          #+#    #+#             */
/*   Updated: 2024/12/11 20:09:06 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uploadHandler.hpp"

uploadHandler::uploadHandler(){}

uploadHandler::uploadHandler(int sock) : _sock(sock) {}

void uploadHandler::responde(request * entry){
    std::cout << "   -----executing UPLOAD request-----" << std::endl;
    //Cambiar!
    std::string downi= "./downloads/"+ fileName(entry);
    std::ofstream ofs(downi.c_str()); // asi solo sube a esa carpeta o puede escogerse?
    ofs << fileContent(entry);
    ofs.close();
    //crear respuesta MIME: cabecera, content-type y contenido y mandar al socket
    response toClient(_sock, "200", "uploaded.html");
    toClient.reply("Uploaded file: " + fileName(entry)); // devolver el nombre del archivo subido
    std::cout << "   -----executed UPLOAD DONE-----" << std::endl;
}

std::string uploadHandler::fileContent(request * entry){ //parsear desde rawRequest o desde request_body de entry
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
    
   // std::string boundary = entry->header_fields["Content-Type"].substr(entry->header_fields["Content-Type"].find("boundary=") + 9, entry->header_fields["Content-Type"].find(CRLF));
    //from = static_cast<postRequest *>(entry)->request_body.find("--" + boundary);
    //end = static_cast<postRequest *>(entry)->request_body.find_last_of("--" + boundary);

    from = static_cast<postRequest *>(entry)->request_body.find("filename=\"") + 10;
    end = static_cast<postRequest *>(entry)->request_body.find("\"", from);
    file_name = static_cast<postRequest *>(entry)->request_body.substr( from, end - from);
    return file_name;
}