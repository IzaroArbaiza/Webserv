/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:31 by iarbaiza          #+#    #+#             */
/*   Updated: 2024/12/19 19:22:37 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfFile.hpp"
#include <algorithm>
#include <sys/stat.h> 
//#include "./cserver.hpp"

ConfFile::ConfFile(const std::string path) : _path(path) {
    _numServ = 0;
}
ConfFile::~ConfFile() {}

int ConfFile::checkFile(const std::string& path) {
    struct stat info;

    // Verifica si el archivo o directorio existe
    if (stat(path.c_str(), &info) != 0) {
        return (-1);  //No se pudo acceder a la ruta
    }

    // Verifica si la ruta tiene al menos 5 caracteres para comparar la extensión
    if (path.size() >= 5) {
        // Obtiene los últimos 5 caracteres de la ruta
        std::string extension = path.substr(path.size() - 5);
        if (extension == ".conf") {
            // Verifica si es un archivo que se puede abrir
            std::ifstream file(path.c_str());
            if (file) {
                return (0); //Es un archivo .conf válido
            } else {
                return (1); //No se pudo abrir el archivo .conf
            }
        }
    }

    // Verifica si es un directorio
    if (info.st_mode & S_IFDIR) {
        return (2);
    }

    // Si no es ni un archivo .conf ni un directorio
    return (3);
}

std::string ConfFile::readFile(std::string file) {
    std::ifstream conf_file(file.c_str());
    if (!conf_file || !conf_file.is_open()) {
        return (NULL);
    }

    std::string content;
    std::string new_file;
    while (std::getline(conf_file, new_file)) {
        content += new_file + "\n";
    }

    return (content);
}

void ConfFile::removeTrash(std::string &content) {
    size_t pos;
    size_t end;

    pos = content.find("#");
    while  (pos < content.length()) {
        end = content.find("\n", pos);
        if (pos >= content.length()) {
            end = content.length();
        }
        content.erase(pos, end-pos);
        pos = content.find('#', pos);
    }
    //Cambiar!! posibility to erase comments (de momento no creo que necesitemos)
}

/**
 * Split de cada uno de los servers del archivo de configuracion
 * Los guarda en un vector de la misma clase _serv
 * Ademas guarda la cantidad de servers encontrados en _numServ
*/
void ConfFile::splitServers(std::string &content) {
    size_t start = 0;
    size_t end = 1;

    if (content.find("server", 0) == std::string::npos) {
        std::cout << "Server not found." << std::endl;
    }

    while (start != end && start < content.length()) {
        if (startServer(content, start) == content.length()) {
            break ;
        }
        start = startServer(content, start);
        end = endServer(content, start);
		if (end <= start) {
			throw(std::invalid_argument("algo va mal"));
		}
		this->_serv.push_back(content.substr(start, end - start + 1));
        this->_numServ++;
        start = end + 1;
    }

	for (std::vector<std::string>::size_type i = 0; i < _serv.size(); ++i) {
        std::cout << "Server num " << i << ": " << _serv[i] << std::endl;
    }
}

//Detecta el inicio de cada server y lo devuelve para la funcion splitServers
size_t ConfFile::startServer(std::string &content, size_t i) {
    size_t start = i;

    while (start < content.length()) {
        if (content[start] == 's') {
            break;
        }
        if (!isspace(content[start])) {
			throw  std::invalid_argument("Wrong character out of server scope{}");
        }
        start++;
    }
    if (start >= content.length()) {
        return (content.length());
    }
    if (content.compare(start, 6, "server") != 0) {
		std::cout << "It's not a server" << std::endl;
        return (content.length());
    }
    start += 6;
	while (start < content.length() && isspace(content[start])){
		start++;
	}
    if (start < content.length() && content[start] == '{') {
        return (start);
    } else {
        std::cout << "nada, encontrado [" << content[start] << "]" << std::endl;
        return (content.length());
    }
}

//Detecta el final de cada server y lo devuelve para la funcion splitServers
size_t ConfFile::endServer(std::string &content, size_t i) {
    size_t end = i;
    size_t j = 0;

    for (end = i + 1; end < content.length(); end++) {
        if (content[end] == '{') {
            j++;
        }
        if (content[end] == '}') {
            if (j == 0) {
                return (end);
            }
            j--;
        }
    }
    return (content.length());
}

/**
 * Checkeo de cada servidor, lanza serverParse para parsearlos, y mas adelante verificar esto mismo
*/
void ConfFile::checkData(std::vector<configuration> &confis) {
    if (_serv.size() != _numServ) {
        throw(std::invalid_argument("Something happened with the server amount."));
    }
    //std::cout << _numServ << std::endl;
    if (_numServ > 1) {
        checkServ();
    }
    for (size_t i = 0; i < _numServ; i++) {
        configuration confis2;
        
        confis2.numServs = _numServ;
        confis2 = serverParse(_serv[i], confis2);
        confis.push_back(confis2);
    }
}

void ConfFile::checkServ() {
    ServerConf serv;
    for (std::vector<std::string>::iterator it1 = _serv.begin(); it1 != _serv.end(); ++it1) {
        for (std::vector<std::string>::iterator it2 = it1 + 1; it2 != _serv.end(); ++it2) {
            if (*it1 == *it2) {
                throw std::invalid_argument("Error: Servers are duplicated.");
            }
        }
    }
}

/**
 * Funcion de parseo de cada una de las lineas de cada servidor del archivo de configuracion
*/
configuration ConfFile::serverParse(std::string &conf, configuration &confis) {
    std::vector<std::string> param;
    std::vector<std::string> errors;
    std::string delimit;
    int loc = 1;
    ServerConf server;

    delimit = " \n\t";
    param = splitParam(conf, delimit);
    if (param.size() < 3) {
        throw(std::invalid_argument("Invalid server parameters"));
    }
    std::cout << "------ Todo bien de momento ------" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; param.size() > i; i++) {
        if (param[i] == "listen" && (i+1) < param.size() && loc) {
            server.dividePortHost(param[++i]);
        }
        else if (param[i] == "server_name" && (i+1) < param.size() && loc) {
            if (!server.getServerName().empty()) {
                throw(std::invalid_argument("Server_name is duplicated"));
            }
            server.setServerName(param[++i]);
        }
        else if (param[i] == "error_page" && (i+1) < param.size() && loc) {
            i+=1;
            while (i < param.size()) {
                errors.push_back(param[i]);
                if (param[i].find(';') < param.size()) {
                    break ;
                }
                if (i+1 >= param.size()) {
                    throw(std::invalid_argument("Wrong error page name"));
                }
                i++;
            }
        }
        else if (param[i] == "client_max_body_size" && (i+1) < param.size() && loc) {
            if (server.getClientMaxBodySize() != 0) {
                throw(std::invalid_argument("Client_max_body_size is duplicated"));
            }
            server.setClientMaxBodySize(param[++i]);
        }
        else if (param[i] == "root" && (i+1) < param.size() && loc) {
            if (!server.getRoot().empty()){
                throw(std::invalid_argument("Root is duplicated"));
            }
            server.setRoot(param[++i]);
        }
        else if (param[i] == "index" && (i+1) < param.size() && loc) {
            if (!server.getIndex().empty()) {
                throw(std::invalid_argument("Index is duplicated"));
            }
            server.setIndex(param[++i]);
        }
        else if (param[i] == "location" && (i+1) < param.size()) {
            std::vector<std::string> code;
            std::string path;
            
            i++;
            if (param[i] == "{" || param[i] == "}") {
                throw(std::invalid_argument("Wrong character in scope."));
            }
            path = param[i];
            if (param[++i] != "{") {
                throw(std::invalid_argument("Wrong ctaracter in scope."));
            }
            i++;
            while (param[i] != "}" && i < param.size()) {
                code.push_back(param[i++]);
            }
            server.setLocation(code, path);
            if (param[i] != "}" && i < param.size()) {
                throw(std::invalid_argument("Wrong character in scope."));
            }
            loc = 0;
        }
        else if (param[i] != "}" && param[i] != "{") {
            if (!loc) {
                std::cout << param[i] << std::endl;
                throw(std::invalid_argument("Wrong parameters after location."));
            } else {
                std::cout << param[i] << std::endl;
                throw(std::invalid_argument("Wrong."));
            }
        }
    }
    if (server.getHost().empty()) {
        server.setHost("localhost;");
    }
    if (server.getRoot().empty()) {
        server.setRoot("/;");
    }
    if (server.getIndex().empty()) {
        server.setIndex("index.html");
    }
    if (!server.getPort()) {
        throw(std::invalid_argument("Port not found"));
    }
    if (server.checkFileHtml(server.getRoot(), server.getIndex())) {
        throw std::invalid_argument("Something went wrong with file");
    }
    if (server.checkLocations()) {
        throw std::invalid_argument("Location is duplicated");
    }
    server.setErrorPages(errors);
    if (!server.validErrorPages()) {
        throw std::invalid_argument("Invalid error or error page");
    }
    
    confis.body_size = server.getClientMaxBodySize();
    confis.host = server.getHost();
    confis.index = server.getIndex();
    confis.port = to_string(server.getPort());
    confis.root = server.getRoot();
    confis.server_name = server.getServerName();
    
    return (confis);
}

/**
 * Split para vector de string donde está todo el servidor dentro, desde { a }
 * - Separa por " " espacios, "\n" saltos de linea y "\t" tabulaciones (evitando guardar ninguna de estas)
*/
std::vector<std::string> ConfFile::splitParam(std::string conf, std::string delimit) {
    size_t start = 0;
    size_t end;
    std::vector<std::string> result;
    std::string tmp;

    while (start < conf.size()) {
        end = conf.find_first_of(delimit, start);
        if (conf.size() == end) {
            break ;
        }
        tmp = conf.substr(start, end-start);
        result.push_back(tmp);
        start = conf.find_first_not_of(delimit, end);
        if (conf.size() == end) {
            break ;
        }
    }
    return (result);
}

std::string	ConfFile::getPath() {
	return (this->_path);
}

size_t ConfFile::get_numServ() {
    return (this->_numServ);
}
