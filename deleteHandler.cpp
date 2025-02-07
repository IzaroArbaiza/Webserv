#include "deleteHandler.hpp"

deleteHandler::deleteHandler(){}

deleteHandler::deleteHandler(int sock) : _sock(sock) {}

void deleteHandler::responde(request* entry, configuration conf){
    std::cout << "   -----responding delete -----" << std::endl;
    std::string path;
    path = entry->getPath();
    //path = "." + path;
    char* cwd = getcwd(NULL,0);
    path = conf.root + path;
    
    
    std::cout << "changing active dir to: " << path << std::endl;
    chdir(path.c_str());
    struct stat sb;
    //if (stat(path.c_str(), &sb) == -1) {
    if (stat(getcwd(NULL,0), &sb) == -1) {
        // Error al obtener información del objeto ¿set errno?
        std::cout << "Error deleting!"<< std::endl;
    } else {
        if ((sb.st_mode & S_IRUSR) == S_IRUSR){ // S_ISDIR(sb.st_mode) es directorio
            std::string file = static_cast<getRequest*>(entry)->getFileName();
            std::cout << "deleteHandler::responde. Deleting: " << static_cast<getRequest*>(entry)->getFileName() << std::endl;
            chown(file.c_str(), 777, 0);
            remove(file.c_str());
        }
        else {
            response toClient(_sock, "403", conf); //forbiden
            toClient.reply(conf);
        }
    }
    //else
    //    std::cout << "Delete Error to handle" << std::endl;
    chdir(cwd);
    //free(cwd);
    //reponse to client
    response toClient(_sock, "204", conf); //¿por qué no carga la pagina delete_html?!!
    toClient.reply(conf);
    std::cout << "   -----responding delete DONE-----" << std::endl;
}
