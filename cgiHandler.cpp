/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:21:35 by xbasabe-          #+#    #+#             */
/*   Updated: 2024/12/04 11:46:49 by xbasabe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

cgiHandler::cgiHandler(){}

cgiHandler::cgiHandler(int sock) : _sock(sock) {}

cgiHandler::cgiHandler(int sock, std::map<std::string, std::string> conf) : _sock(sock), cgiconfig(conf) {}

void cgiHandler::responde(request * entry){
    std::string file_name;
    std::string file_type;
    size_t from;
    size_t end;
    std::string file_content;
    std::string file;

    std::cout << "   -----executing CGI handler-----" << std::endl;

    std::cout << "initialized CGI handler" << std::endl;
    std::cout << "    filename: " << cgiFileName(entry) << std::endl;
    std::cout << "    vbles: " << vblesMethod(entry) << std::endl;
    std::cout << "      request - domain: " << entry->getDomain() << std::endl;
    std::cout << "      request - uri: " << entry->getUri() << std::endl;
    std::cout << "      request - query string: " << entry->getQueryString()<< std::endl;
    std::cout << "      request - path: " << entry->getPath()<< std::endl;
    std::cout << "      request - resource: " << entry->getResource()<< std::endl;

    int fd[2]; //pipe a fd for STDIN_FILENO. cgi -> webserver
    pipe(fd);
    int pid = fork();
    
    //std::cout << "absolute path: " << buildPath(entry)<< std::endl;
    //parse_resource(entry); //pasado a los get post request

    if (pid < 0) {
        std::cerr << "Error: fork() failed" << std::endl;
        response toClient(_sock, "500", cgiconfig);
        toClient.reply();
        exit(1);// es necesario? o como ya damos el error 500 vale?
    }
    else if (pid == 0) { // Child process
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        file_name = cgiFileName(entry);
        std::cout << "begining of child process" << std::endl;
        //if (split(buildPath(entry), "/")[1] != "cgi-bin"){ //chequeamos la primera parte par permitir subcarpetas en cgi-bin
        /*if (split(resource, "/")[1] != "cgi-bin"){ 
            std::cout << split(resource, "/")[1] << " no está en folder cgi cgi."<< std::endl;
            response toClient(_sock, "403");
            toClient.reply("out of cgi folder");
            kill(getpid(),SIGINT); //exit con salida =2
            //exit(403);
        }
        else{*/
            if(file_name.substr(file_name.find('.')) == ".py")//python files
                this->execPy(file_name, entry);
            else
                execBinary(file_name, entry);
        //}
    }
    else if (pid > 0){ //parent
    close(fd[1]);
	dup2(fd[0], STDIN_FILENO);

    //read child answer
    std::string buffer;
    buffer.resize(1024);
    std::string cgi_text;
    while (read(fd[0], &buffer[0], 1024) > 0){
        cgi_text.append(buffer);
    }
    dup2(STDIN_FILENO, 0);

    //check error in child execution
    int status;
    waitpid(pid, &status, 0); // Wait for child process to terminate
    if (WIFSIGNALED(status)) {
        if (status == 2){ //error forzado si se accede a un cgi fuera de la carpeta cgi-bin
            response toClient(_sock, "403");
            toClient.reply();
        }
        else{
        std::string output = "Child process terminated due to signal:";
        output.append(std::to_string(status));
        std::cout << "Child process terminated due to signal:" << status << std::endl;
        response toClient(_sock, "500", cgiconfig);
        toClient.reply(); //¿meter aqui el fallo: output
        }
        close(fd[0]);
    }
    else{
        std::cout << "Child process terminated normally" << std::endl;
        response toClient(_sock, "200", "cgi_page.html");
        toClient.reply(cgi_text);
        close(fd[0]);
        }
    }
    std::cout << "   -----executed CGI DONE-----" << std::endl;
}

std::string cgiHandler::buildPath(request * entry){
    //Si necesitamos acceder a diferentes carpetas. 
    //return root + entry->getPath() + "/" + cgiFileName(entry); //Acceder a root, raiz de ficheros del servidor, no a domain
    std::cout << "cgiHandler:buildpath test: " << cgiconfig["root"] + entry->getPath() + "/" + cgiFileName(entry) << std::endl;
    std::cout << "cgiHandler:buildpath test 2: " << "." + entry->getPath() + "/" + cgiFileName(entry) << std::endl;
    //return "." + entry->getPath() + "/" + cgiFileName(entry);
    return cgiconfig["root"] + entry->getPath() + "/" + cgiFileName(entry); //desde root
}

std::string cgiHandler::envPath(request * entry){
    std::cout << "cgiHandler:envPath test: " << "PATH=" + cgiconfig["root"] + entry->getPath() << std::endl;
    //return "." + entry->getPath() + "/" + cgiFileName(entry);
    return "PATH=" + cgiconfig["root"] + entry->getPath();
}

void cgiHandler::execBinary(std::string file_name, request *entry){
    std::string path = "./cgi-bin/" + file_name; //ruta relativa, solo cgis que esten en la carpeta cgi-bin
    std::string vbles = static_cast<getRequest *>(entry)->getVbles();
    char *envp[] = { NULL };
    //char *envp[] = {(char *)envPath(entry).c_str(), NULL }; //pasar env?
    if (vbles.empty()){
        char *argv[] = {(char *)path.c_str(), (char *)file_name.c_str(), NULL};
        execve(argv[0], &argv[0], envp);
    }
    else {
       char *argv[] = {(char *)path.c_str(), (char *)file_name.c_str(), (char *)vbles.c_str(), NULL};
        execve(argv[0], &argv[0], envp);
    }
}

void cgiHandler::execPy(std::string file_name, request *entry){
    std::string path = "./cgi-bin/" + file_name;
    std::string vbles = vblesMethod(entry);
    char *envp[] = { NULL };
    if (vbles.empty()){
        char *argv[] = { "/usr/bin/python3",(char *)path.c_str(),  NULL };
        execve(argv[0], &argv[0], envp);
    }
    else{
        char *argv[] = { "/usr/bin/python3",(char *)path.c_str(), (char *)vbles.c_str(),  NULL };
        execve(argv[0], &argv[0], envp);
    }
}

std::string cgiHandler::cgiFileName(request* entry){
    if (entry->getMethod() == "POST"){
        return static_cast<postRequest *>(entry)->getFileName();
    }
    else if(entry->getMethod() == "GET"){
        return static_cast<getRequest *>(entry)->getFileName();
    }
}

std::string cgiHandler::vblesMethod(request *entry)
{
    if (entry->getMethod() == "GET"){
        return static_cast<getRequest *>(entry)->getVbles();
    }
    if (entry->getMethod() == "POST"){
        return static_cast<postRequest *>(entry)->getVbles();
    }
}

/*
void cgiHandler::parse_resource(request * entry){
    std::cout << "cucu cgi parse_resource 1"<< std::endl;
    resource = getcwd(NULL,0);
    resource = resource + "/" + static_cast<getRequest *>(entry)->getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    std::cout << "cucu cgi parse_resource 2"<< std::endl;
    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        resource = resource + "/" + static_cast<getRequest *>(entry)->getFileName();
    std::cout << "request init. resource: " << resource << std::endl;
}*/