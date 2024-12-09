#include "deleteHandler.hpp"

deleteHandler::deleteHandler(){}

deleteHandler::deleteHandler(int sock) : _sock(sock) {}

void deleteHandler::responde(request* entry){
    std::cout << "   -----responding delete -----" << std::endl;
    std::string path = entry->getPath();
    char* cwd = getcwd(NULL,0);
    //path = root + path; //si metemos un root desde el archivo de configuración. Raiz de los ficheros de webserver
    path = "." + path;
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
            chown(file.c_str(), 777, 0);
            remove(file.c_str());
        }
        else {
            response toClient(_sock, "403"); //forbiden
            toClient.reply();
        }
    }
    //else
    //    std::cout << "Delete Error to handle" << std::endl;
    chdir(cwd);
    //free(cwd);
    //reponse to client
    response toClient(_sock, "204"); //¿por qué no carga la pagina delete_html?!!
    toClient.reply();
    std::cout << "   -----responding delete DONE-----" << std::endl;
}
