#include "getHandler.hpp"

getHandler::getHandler(){}

getHandler::getHandler(int sock) : _sock(sock) {
}

getHandler::getHandler(int sock, std::map<std::string, std::string>  conf) : _sock(sock){
    gethandlerconfig = conf;
}

void getHandler::responde(request * entry){
    std::cout << "    -----getHandler ---- request-" << "URI: " << entry->getUri() << std::endl;
    std::cout << "    -----getHandler ---- request-" << "Location: " << static_cast<getRequest *>(entry)->getLocation() << std::endl;
    std::cout << "    -----getHandler ---- request-" << "resource: " << static_cast<getRequest *>(entry)->getResource() << std::endl;    

    parse_resource(entry); //pasado a los get y post request. resource en request
    
    //eliminar?? 418 no es oficia
    
    if (static_cast<getRequest *>(entry)->redir_index){
    //if (entry->check_index()){
        std::cout << "Es una redirección" << std::endl;
        response toClient(_sock, "308");
        toClient.redirect("index.html");
    }
    else if (accesable(entry) == "no"){
        std::cout << "NO accesible resource" << std::endl;
        response toClient(_sock, "404", gethandlerconfig);
        toClient.reply();
    }
    //else if (!entry->isFolder("." + static_cast<getRequest *>(entry)->getPath()) && entry->getExtention() != ".html"){
    //else if (!entry->isFolder(static_cast<getRequest *>(entry)->getPath()) && entry->getExtention() != ".html"){
    else if (accesable(entry) == "folder"){
        DIR *dir;
        struct dirent *ent;
        std::string path = entry->getResource();
        //std::string path = resource;
        std::cout << "accesing folder: " << resource << std::endl;
        std::ofstream fichero("listado", std::ios::out);
        dir = opendir(path.c_str());
        if (dir == NULL) {
            std::cout << "Error opening the directory" << std::endl;
        }
        else{
        fichero << "<html><code>";
        while ((ent = readdir(dir)) != NULL) {
            //fichero << "<ul>" << ent->d_name << "</ul>" << std::endl; listado no clickable
            fichero << "<ul>" << "<a href=" << static_cast<getRequest*>(entry)->getLocation () << "?" << ent->d_name << ">"  << ent->d_name <<"</a>" << "</ul>";
            //fichero << "<ul>" << "<a href=" << entry->getResource() << "?" << ent->d_name << ">"  << ent->d_name <<"</a>" << "</ul>";
            //fichero << "<ul>" << "<a href=" << resource << "?" << ent->d_name << ">"  << ent->d_name <<"</a>" << "</ul>";
        }
        fichero << "</code></html>";
        }
        closedir(dir);
        fichero.close();
        response toClient(_sock, "200", "./listado"); //cgi necesita resource no location
        toClient.reply();
    }
    
    else if (entry->getUri().find("/downloads?file=") != std::string::npos){ //delete deberia llegar como metodo=delete
        std::cout << "delete biassed by get" << std::endl;
        deleteHandler Handler(_sock);
        Handler.responde(entry);
    }
    else if (isExecutable(entry)){
        std::cout << "getHandler: CGI" << std::endl;
        cgiHandler Handler(_sock, gethandlerconfig);
        Handler.responde(entry);
    }
    else{
        if (access(static_cast<getRequest *>(entry)->getLocation().c_str(),R_OK) == -1){ //ver permisos ¿y por grupos?
        //if (access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1){
            std::cout << "Error: " << strerror(errno);
            response toClient(_sock, "404", gethandlerconfig);
            toClient.reply();
        }
        else{
            //llega aqui alguna vez la redirección?
            if (static_cast<getRequest *>(entry)->redir_index){
                std::cout << "redirected to index.html" << std::endl;
                response toClient(_sock, "308");
                toClient.redirect("index.html");
                }
            else{
                std::cout << "getHandler reposnde location: " << static_cast<getRequest *>(entry)->getLocation() << std::endl;
                response toClient(_sock, "200", static_cast<getRequest *>(entry)->getLocation()); //cgi necesita resource no location
                toClient.reply();
            }
        }
    }
    std::cout << "    -----responding GET DONE-----" << std::endl;
}

void getHandler::parse_resource(request * entry){
    //resource = getcwd(NULL,0); //aqui se crearia el enlace desde config["root"]
    resource = gethandlerconfig["root"];
    std::cout << "getHandler parse_resource root: " << resource << std::endl;
    resource = resource + "/" + static_cast<getRequest *>(entry)->getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        resource = resource + "/" + static_cast<getRequest *>(entry)->getFileName();
    std::cout << "request init. resource: " << resource << std::endl;
}

bool getHandler::isExecutable(request * entry){
    //std::string check = entry->getDomain() + entry->getPath() + static_cast<getRequest *>(entry)->getFileName(); Cambiar domain por root
    std::string check = getcwd(NULL,0); //con root... chekearia cualquier host que introduzca el conf file?
    //check = check + "/" + static_cast<getRequest *>(entry)->getLocation() + "/" + static_cast<getRequest *>(entry)->getFileName();

    check = check + "/" + static_cast<getRequest *>(entry)->getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        check = check + "/" + static_cast<getRequest *>(entry)->getFileName();

    std::cout << "getHanlder  isExecutable  check: " << check << std::endl;
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        std::cout << "Error al obtener información del archivo. Error stat" << std::endl;
        return false;
    }
    //else if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR)){
    else if ((sb.st_mode & S_IXUSR) == S_IXUSR){ //no comprobamos si es es folder ¿puede ser un folder ejecutable?
           std::cout << "getHandler isExecutable: IS executable" << std::endl;
            return true;
        }
    else {
        std::cout << "getHandler isExecutable: is NOT executable" << std::endl;
        return false;
    }
}

/*
std::string getHandler::file_type(std::string route){
    struct stat st;
    route = getcwd(NULL,0) + route;
    std::cout << "request.isFolder() route: " << route  << ". is a ";
    if (stat(route.c_str(), &st) == -1) {
        std::cout << "request isFolder. Error stat" << std::endl;
        return "error";
    } else {
        if (S_ISDIR(st.st_mode)) {
            std::cout << "FOLDER" << std::endl;
            return "folder";
        } else{
            if (S_ISREG(st.st_mode)){ // es fichero regular
                std::cout << "regular FILE" << std::endl;
                if (st.st_mode & S_IXUSR) //tiene permisos de ejecucion
                    //std::cout << "EXECUTABLE file" << std::endl;
                    return "executable";
                else
                    return "file";
            }
        }
    }
}*/

std::string getHandler::accesable(request * entry){
    //std::string check = getcwd(NULL,0); //con root... chekearia cualquier host que introduzca el conf file?
    /*
    check = check + "/" + static_cast<getRequest *>(entry)->getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        check = check + "/" + static_cast<getRequest *>(entry)->getFileName();
    */
    std::string check = entry->getResource();
    std::cout << "accesable path: " << check << std::endl;
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        std::cout << "accesable stat error" << std::endl;
        return "no";
    }
    else if (S_ISDIR(sb.st_mode)){ //no comprobamos si es es folder ¿puede ser un folder ejecutable?
        std::cout << "accesable: folder"<< std::endl;
        //if (check == entry->getDomain())
        //    return true;
        return "folder";
    }
    else if ((sb.st_mode & S_IXUSR) == S_IXUSR){ //no comprobamos si es es folder ¿puede ser un folder ejecutable?
        std::cout << "accesable: exec"<< std::endl;
        return "exec";
    }
    else if (S_ISREG(sb.st_mode) && check.find("html") != std::string::npos){
        std::cout << "accesable: page"<< std::endl;
        return "page";
    }
    else if (S_ISREG(sb.st_mode) && check.find("ico") != std::string::npos){
        std::cout << "accesable: ico"<< std::endl;
        return "ico";
    }
    else if (S_ISREG(sb.st_mode) && check.find("png") != std::string::npos){
        std::cout << "accesable: png"<< std::endl;
        return "png";
    }
    else if (S_ISREG(sb.st_mode) && check.find("jpg") != std::string::npos){
        std::cout << "accesable: jpg"<< std::endl;
        return "jpg";
    }
    std::cout << "accesable: default NO"<< std::endl;
    return "no";
}
