#include "postHandler.hpp"

postHandler::postHandler(){}

postHandler::postHandler(int sock) : _sock(sock) {}

void postHandler::responde(request * entry){
    std::string file_name;
    std::string file_type;
    //size_t from;
    //size_t end;
    std::string file_content;
    std::string file;

    std::cout << "   -----responding POST request-----" << std::endl;

    if (actionDetector(entry) == "cgi"){
    //if (isExecutable(entry)){
        std::cout << "postHandler: CGI" << std::endl;
        execute(entry);
        //cgiHandler Handler(_sock);
        //Handler.responde(entry);
    }
    else if (actionDetector(entry) == "upload"){
        std::cout << "postHandler: UPLOAD" << std::endl;
        uploadHandler Handler(_sock);
        Handler.responde(entry);
    }
    else if (actionDetector(entry) == "delete"){ //debe entrar como metodo, implementado y probado con curl
        std::cout << "postHandler: DELETE" << std::endl;
        deleteHandler Handler(_sock);
        Handler.responde(entry);
    }
    else{   //html request 
        std::cout << "postHandler: HTML request" << std::endl;
        if (access(static_cast<postRequest *>(entry)->getLocation().c_str(),R_OK) == -1){ //ver permisos ¿y por grupos?
        //if (access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1){
            std::cout << "Error: " << strerror(errno);
            response toClient(_sock, "404");
            toClient.reply();
        }
        else{
            //response toClient(_sock, "200", static_cast<getRequest *>(entry)->getResource());
            response toClient(_sock, "200", static_cast<postRequest *>(entry)->getLocation()); //cgi necesita resource no location
            toClient.reply();
        }
    }
    std::cout << "   -----responding POST DONE-----" << std::endl;
}

std::string postHandler::actionDetector(request * entry)
{
    std::cout << "postHandler ruta al recurso: " <<  "./cgi-bin/" + static_cast<postRequest *>(entry)->getFileName() << std::endl;
    std::string type = entry->header_fields["Content-Type"];
    if (type.find("multipart/form-data") != std::string::npos)
        //if (entry->getUri().find("/downloads/upload") != std::string::npos)
            return "upload";
        //else
        //    return "delete";
    else if (type.find("application/x-www-form-urlencoded") != std::string::npos)
        return "cgi";
    return (0);
}

void postHandler::execute(request * entry){
    std::string check = "./cgi-bin/" + static_cast<postRequest *>(entry)->getFileName();
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        std::cout << "Error al obtener información del archivo" << std::endl;
        //error 406 Not Acceptable?
        response toClient(_sock, "403"); //El 403 El código de estado de error prohibido
        toClient.reply();
    } else {
        //if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR) || entry->getExtention() == ".py") { //miramos permiso del user, ¿mirar otros permisos?
        if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR)) {
            cgiHandler Handler(_sock);
            Handler.responde(entry);
        } else {
            response toClient(_sock, "403"); //El 403 El código de estado de error prohibido
            toClient.reply();
        }
    }
}

std::string postHandler::file_type(std::string route){
    struct stat st;
    route = getcwd(NULL,0) + route;
    std::cout << "request.isFolder() route: " << route  << ". is a ";
    if (stat(route.c_str(), &st) == -1) {
        // Error al obtener información del objeto ¿set errno?
        std::cout << "request isFolder. Error stat" << std::endl;
        return "error";
    } else {
        if (S_ISDIR(st.st_mode)) {
            std::cout << "FOLDER" << std::endl;
            return "folder";
        //if (S_ISREG(st.st_mode)) es fichero regular si ademas tiene permisos es ejecutable && (sb.st_mode & S_IXUSR)
        }
        else{
            //std::cout << "is NOT a folder" << std::endl;
            if (S_ISREG(st.st_mode)){ // es fichero regular
                std::cout << "regular FILE" << std::endl;
                if (st.st_mode & S_IXUSR)
                    //std::cout << "EXECUTABLE file" << std::endl;
                    return "executable";
                else
                    return "file";
             //si ademas tiene permisos es ejecutable && (sb.st_mode & S_IXUSR)
            }
        }
    }
    return (0);
}
