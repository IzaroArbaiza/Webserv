/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:42 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:43 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/deleteHandler.hpp"

deleteHandler::deleteHandler(){}

deleteHandler::deleteHandler(int sock) : _sock(sock) {}

void deleteHandler::responde(request* entry, configuration conf){
    std::string path;
    path = entry->getPath();
    char* cwd = getcwd(NULL,0);
    path = conf.root + path;
    
    
    chdir(path.c_str());
    struct stat sb;
    if (stat(getcwd(NULL,0), &sb) == -1) {
        std::cout << "Error deleting!"<< std::endl;
    } else {
        if ((sb.st_mode & S_IRUSR) == S_IRUSR){
            std::string file = static_cast<getRequest*>(entry)->getFileName();
            chown(file.c_str(), 777, 0);
            remove(file.c_str());
        } else {
            response toClient(_sock, "403", conf);
            toClient.reply(conf);
        }
    }
    chdir(cwd);
    response toClient(_sock, "204", "/pages/deleted.html");
    toClient.reply(conf);
}
