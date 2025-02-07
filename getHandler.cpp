#include "getHandler.hpp"

getHandler::getHandler(){}

getHandler::getHandler(int sock) : _sock(sock) {
}

void getHandler::responde(request * entry, configuration  conf){
    //std::cout << "    -----getHandler ---- request-" << "URI: " << entry->getUri() << std::endl;
    //std::cout << "    -----getHandler ---- request-" << "Location: " << static_cast<getRequest *>(entry)->getLocation() << std::endl;
    std::cout << "    -----getHandler ---- request-" << "resource: " << static_cast<getRequest *>(entry)->getResource() << std::endl; 

    //parse_resource(entry, conf); //aqui es donde cambiamos la ruta  con /algo o sin
    static_cast<getRequest *>(entry)->parse_resource(conf);

    std::string resource_type = accesable(entry);

    if (resource_type == "error"){
    //if( access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1 ){
        std::cout << "Error accesing the resoure. error 404: " << conf.errors[404] << std::endl;
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
    }

    else if (resource_type == "no"){
    //if( access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1 ){
        std::cout << "NO accesible resource -> error 404: " << conf.errors[404] << std::endl;
        response toClient(_sock, "404", conf);
        toClient.reply(conf);
    }
    //else if (!entry->isFolder("." + static_cast<getRequest *>(entry)->getPath()) && entry->getExtention() != ".html"){
    //else if (!entry->isFolder(static_cast<getRequest *>(entry)->getPath()) && entry->getExtention() != ".html"){
    else if (resource_type == "folder"){
        std::cout << "getHanlder for folder request. autoindex: " << conf.autoindex << std::endl; //mirar parseo del autoindex
        if (conf.autoindex) //si el autoindex esta ON aunque haya index.html se muestra el contenido de la carpeta
            autoindex(entry, conf);
        else // si el autoindex esta OFF buscamos si existe index.html
            index_in_folder(entry, conf);  //in folder o en la configuracion de la location, falta en locations
    }
    
    else if (isExecutable(entry, conf)){
        std::cout << "getHandler: CGI" << std::endl;
        cgiHandler Handler(_sock);
        Handler.responde(entry, conf);
    }
    else{
        //if (access(static_cast<getRequest *>(entry)->getLocation().c_str(),R_OK) == -1){ //ver permisos ¿y por grupos?
        if (access(static_cast<getRequest *>(entry)->getResource().c_str(),R_OK) == -1){
            //std::cout << "Error: " << strerror(errno);
            std::cout << "getHandler permisos if(access)" << std::endl;
            response toClient(_sock, "403", conf);
            toClient.reply(conf);
        }
        else{
            //llega aqui alguna vez la redirección?
            /*
            if (static_cast<getRequest *>(entry)->redir_index){
                std::cout << "redirected to index.html" << std::endl;
                response toClient(_sock, "308");
                toClient.redirect(conf.index, conf);
                }
            else*/{
                std::cout << "getHandler::responde. uri: " << static_cast<getRequest *>(entry)->getUri() << std::endl;
                std::cout << "getHandler::responde. resource: " << static_cast<getRequest *>(entry)->getResource() << std::endl;
                if (static_cast<getRequest *>(entry)->getLocation() == "favicon.ico")
                    response toClient(_sock, "200", static_cast<getRequest *>(entry)->getUri());
                response toClient(_sock, "200", static_cast<getRequest *>(entry)->getResource());
                toClient.reply(conf);
            }
        }
    }
    std::cout << "    -----responding GET DONE-----" << std::endl;
}

void getHandler::parse_resource(request * entry, configuration conf){
    /*
    resource = conf.root;
    std::cout << "getHandler parse_resource root: " << resource << std::endl;
    //resource = resource + "/" + static_cast<getRequest *>(entry)->getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    if (!static_cast<getRequest *>(entry)->getFileName().empty())
        resource = resource + "/" + static_cast<getRequest *>(entry)->getFileName();
        */
    resource = conf.root + entry->getUri();
    std::cout << "getHandler::parse resource. resource: " << resource << std::endl;
}

bool getHandler::isExecutable(request * entry, configuration conf){ // ¿cambiar por es una extension valida en la location cgi-bin? caso ejecutable pero no en la location?
    std::string check = static_cast<getRequest *>(entry)->getResource(); (void) conf;
    //std::string check = conf.root + "/" + static_cast<getRequest *>(entry)->getFileName();
    //if (!static_cast<getRequest *>(entry)->getFileName().empty())
    //    check = check + "/" + static_cast<getRequest *>(entry)->getFileName();
    //std::string check = resource; (void) entry; (void) conf;
    std::cout << "getHanlder::isExecutable  check: " << check << std::endl;
    struct stat sb;
    if (stat(check.c_str(), &sb) == -1) {
        std::cout << "Error al obtener información del archivo. Error stat" << std::endl;
        return false;
    }
    //else if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR)){
    else if ((sb.st_mode & S_IXUSR) == S_IXUSR && !S_ISDIR(sb.st_mode)){ //ejecutable y no es folder
           std::cout << "getHandler isExecutable: IS executable" << std::endl;
            return true;
        }
    else {
        std::cout << "getHandler isExecutable: is NOT executable" << std::endl;
        return false;
    }
}

std::string getHandler::accesable(request * entry){
    //std::string check = entry->getResource(); //si lo queremos parseado a /algo usar check = resource parseado en esta clase por su parse resource linea 19
    std::string check = static_cast<getRequest *>(entry)->getResource();
    
    std::cout << "accesable path: " << check << std::endl;
    struct stat sb;
    
    if (stat(check.c_str(), &sb) == -1) {
        std::cout << "accesable stat error" << std::endl;
        return "error";
    }

    if (S_ISDIR(sb.st_mode)){ //no comprobamos si es es folder ¿puede ser un folder ejecutable? No.
        std::cout << "getHandler::accesable, is folder"<< std::endl;
        //if (check == entry->getDomain())
        //    return true;
        return "folder";
    }
     else if (S_ISREG(sb.st_mode) && check.find("html") != std::string::npos){
        std::cout << "getHandler::accesable, is page"<< std::endl;
        return "page";
    }
    else if ((sb.st_mode & S_IXUSR) == S_IXUSR){ //no comprobamos si es es folder ¿puede ser un folder ejecutable? No.
        std::cout << "getHandler::accesable, is exec"<< std::endl;
        return "exec";
    }

    else if (S_ISREG(sb.st_mode) && check.find("ico") != std::string::npos){
        std::cout << "getHandler::accesable, is ico"<< std::endl;
        return "ico";
    }
    else if (S_ISREG(sb.st_mode) && check.find("png") != std::string::npos){
        std::cout << "getHandler::accesable, is png"<< std::endl;
        return "png";
    }
    else if (S_ISREG(sb.st_mode) && check.find("jpg") != std::string::npos){
        std::cout << "getHandler::accesable, is jpg"<< std::endl;
        return "jpg";
    }
    std::cout << "getHandler::accesable, default ERROR"<< std::endl;
    return "error";
}

void getHandler::autoindex(request *entry, configuration conf){
    DIR *dir;
    struct dirent *ent;
    std::string path = static_cast<getRequest*>(entry)->getResource();
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
    response toClient(_sock, "200", "./listado");
    toClient.reply(conf);
}

void getHandler::index_in_folder(request *entry, configuration conf){
    DIR *dir;
        struct dirent *ent;
        std::string path = static_cast<getRequest*>(entry)->getResource();
        //std::string path = resource;
        std::cout << "researching for index in: " << resource << std::endl;
        std::ofstream fichero("listado", std::ios::out);
        dir = opendir(path.c_str());
        if (dir == NULL) {
            std::cout << "Error opening the directory" << std::endl;
        }
        else{
        fichero << "<html><code>";
        while ((ent = readdir(dir)) != NULL) {
            std::string file = ent->d_name;
            std::string parsed_index = "index.html";
            if (!conf.index.empty())
                parsed_index = conf.index;
            if (!file.compare(parsed_index) ){
                std::cout << "there is an index in folder" << std::endl;
                std::cout << "launching index: " << static_cast<getRequest*>(entry)->getResource() + "/" + ent->d_name<< std::endl;
                response toClient(_sock, "200", static_cast<getRequest*>(entry)->getResource() + "/" + ent->d_name );
                toClient.reply(conf);
                return ;
            }
        }
        fichero << "</code></html>";
        }
        closedir(dir);
        fichero.close();
        
}
