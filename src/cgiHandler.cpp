/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:21:35 by xbasabe-          #+#    #+#             */
/*   Updated: 2025/03/12 18:22:13 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cgiHandler.hpp"

cgiHandler::cgiHandler(){}

cgiHandler::cgiHandler(int sock) : _sock(sock) {}

void cgiHandler::responde(request * entry, configuration conf){
    std::string file_name;
    std::string file_type;
    //std::string file_content;
    std::string file;
    int fd[2];
    pipe(fd);
    int pid = fork();
    
    if (pid < 0) {
        std::cerr << "Error: fork() failed" << std::endl;
        response toClient(_sock, "500", conf);
        toClient.reply(conf);
        exit(1);
    } else if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        file_name = cgiFileName(entry);
        if(file_name.substr(file_name.find('.')) == ".py")
            this->execPy(file_name, entry, conf, ".py");
        else if (file_name.substr(file_name.find('.')) == ".out")
            execBinary(file_name, entry, conf);
        else
            std::cout << "This server only executes .py and .out files" << std::endl;
    } else if (pid > 0){
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        std::string buffer;
        buffer.resize(1024);
        std::string cgi_text;
        while (read(fd[0], &buffer[0], 1024) > 0) {
            cgi_text.append(buffer);
        }
        dup2(STDIN_FILENO, 0);
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            if (status == 2){
                response toClient(_sock, "403");
                toClient.reply(conf);
            } else{
                response toClient(_sock, "500", conf);
                toClient.reply(conf);
            }
            close(fd[0]);
        } else{
            response toClient(_sock, "200", "site_root/pages/cgi_page.html");
            toClient.reply(cgi_text, conf);
            close(fd[0]);
        }
    }
}


void cgiHandler::execBinary(std::string file_name, request *entry, configuration conf){
    (void) conf;
    std::string path = static_cast<getRequest *>(entry)->getResource();
    std::string vbles = static_cast<getRequest *>(entry)->getVbles();
    char *envp[] = { NULL };

    if (vbles.empty()){
        char *argv[] = {(char *)path.c_str(), (char *)file_name.c_str(), NULL};
        execve(argv[0], &argv[0], envp);
    } else {
       char *argv[] = {(char *)path.c_str(), (char *)file_name.c_str(), (char *)vbles.c_str(), NULL};
        execve(argv[0], &argv[0], envp);
    }
}



void cgiHandler::execPy(std::string file_name, request *entry, configuration conf, std::string extension){
    std::string path = entry->getResource();
    (void) file_name;
    std::string executer;
    std::string vbles = vblesMethod(entry);
    char *envp[] = { NULL };
    
    for (size_t i = 0; i < conf.locations.size(); i++){
        if (conf.locations[i].location_name == "/cgi-bin"){
            executer = conf.locations[i].extPath[extension];
            break;
        }
    }
    if (vbles.empty()){
        char *argv[] = { (char *)executer.c_str(),(char *)path.c_str(),  NULL };
        execve(argv[0], &argv[0], envp);
    } else {
        char *argv[] = { (char *)executer.c_str(),(char *)path.c_str(), (char *)vbles.c_str(),  NULL };
        execve(argv[0], &argv[0], envp);
    }
}

std::string cgiHandler::cgiFileName(request* entry){
    if (entry->getMethod() == "POST"){
        return static_cast<postRequest *>(entry)->getFileName();
    } else if(entry->getMethod() == "GET"){
        return static_cast<getRequest *>(entry)->getFileName();
    }
    return (0);
}

std::string cgiHandler::vblesMethod(request *entry)
{
    if (entry->getMethod() == "GET"){
        return static_cast<getRequest *>(entry)->getVbles();
    }
    if (entry->getMethod() == "POST"){
        return static_cast<postRequest *>(entry)->getVbles();
    }
    return (0);
}

std::string cgiHandler::resourceMethod(request *entry)
{
    if (entry->getMethod() == "GET"){
        return static_cast<getRequest *>(entry)->getResource();
    }
    if (entry->getMethod() == "POST"){
        return static_cast<postRequest *>(entry)->getResource();
    }
    return (0);
}

void cgiHandler::to_location_resource(request * entry, configuration conf){
    resource = conf.root;

    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        resource = resource + "/" + static_cast<getRequest *>(entry)->getFileName();
}

