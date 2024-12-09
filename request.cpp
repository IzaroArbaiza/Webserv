#include "request.hpp"

request::request(std::string received) : rawRequest(received){
    parseURL();
    parseHeader();
    checkExtention();
    //std::cout << "initialized request" << std::endl;
    //std::cout << "    method: " << method << std::endl;
    //std::cout << "    scheme: " << scheme << std::endl;
    //std::cout << "    domain: " << domain << std::endl;
    //std::cout << "    uri: " << uri << std::endl;
    //std::cout << "    query string: " << query_string << std::endl;
    //std::cout << "    path: " << path << std::endl;
    //std::cout << "    resource: " << resource << std::endl;
    //std::cout << "    extention: " << extention << std::endl;
}

request::request(){}

request::request(request& cp){
    *this = cp;
}

request & request::operator=(request& cp){
    this->header_fields = cp.header_fields; //resto de variables
    return *this;
}

void request::fill(std::string message){}

std::string request::getMethod(){ return method; }

std::string request::getUri(){ return uri; }

std::string request::getRequestLine(){ return request_line; }

std::string request::getScheme(){ return scheme; }

std::string request::getPath(){ return path; }

std::string request::getExtention(){ return extention; }

std::string request::getQueryString(){ return query_string; }

std::string request::getDomain(){ return domain; }

std::string request::getResource(){ return resource; }

void request::getResource(std::string res){ resource = res; }

request::~request(){
//delete factory objects?
}

void request::parseURL(){ //reescribir usando como ejemplo el curl?
    std::vector<std::string> splited;
    request_line = rawRequest.substr(0, rawRequest.find(CRLF));
    splited = split(request_line, " ");
    if (splited.size() != 3)
        std::cout << "error: incorrect request line" << std::endl; //eror
    method =  splited[0];
    uri=""; //initialize?
    if (splited[1].length() > 1){ //without query string the result of splited[1] is "/"
        uri = splited[1];
        resource = splited[1].substr(splited[1].find_last_of("/"));
        path = splited[1]; //path + query
        path = path.substr(0, path.find("?")); //path es, segun terminologia http, resource y el path de verdad es localhost + resource
        if (splited[1].find("?") != std::string::npos){
            query_string = splited[1].substr(splited[1].find("?") + 1);
        }
        //std::cout << "query string " << query_string << std::endl; // no tenemos en cuenta que podria venir #ancla para ir a una zona de la pagina
    }
    //isFolder(resource);
    scheme = splited[2];
    if (scheme != "HTTP/1.1")
        std::cout << "Error: 505 HTTP Version Not Supported" << scheme << std::endl;
}

void request::parseHeader(){
    std::vector<std::string> lines;
    std::string request_header;
    request_header = rawRequest.substr(rawRequest.find(CRLF), rawRequest.find("\r\n\r\n") - rawRequest.find(CRLF));
    lines = split(request_header, CRLF);
    for (int i = 1; i < lines.size() - 2; i++){
        if (lines[i] == "\r\n\r\n"){ //quitar?
            break ;
        }
        header_fields[lines[i].substr(0,lines[i].find_first_of(":"))] = lines[i].substr(lines[i].find_first_of(":") + 2);
        //std::cout << "request:: ver headers "<< header_fields[lines[i].substr(0,lines[i].find_first_of(":"))] << std::endl;
    }
    domain = header_fields["Host"];
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;

    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start, end - start));
    return tokens;
}

void request::unfolding(std::string &str){
    //Elimiar FWL, definir FWL
    /*FWS (the folding white space
   token) appears, it indicates a place where folding, as discussed in
   section 2.2.3, may take place.  Wherever folding appears in a message
   (that is, a header field body containing a CRLF followed by any WSP),*/
    str.erase(str.find(FWL), str.find(FWL) + 3);
}

void request::checkExtention(){
    if (path.find_last_of(".") != std::string::npos){
        extention = path.substr(path.find_last_of("."));
    }
    else{ //sin extensión, es un folder o teapot
        extention = "exec";
        //std::string ruta = header_fields["Referer"].substr(0, header_fields["Referer"].find("?")) + path;
        std::string ruta = domain + resource;
        std::fstream fs;
        fs.open(ruta, std::ios::in);
        if (fs.fail())
            extention = "folder";
        fs.close();
        std::cout << "extention: " << extention << std::endl;
        if (extention != "folder")
            std::cout << "TEapot my friend" << std::endl;
    }
}

bool request::isFolder(std::string route){
    struct stat st;
    route = getcwd(NULL,0) + route;
    std::cout << "request.isFolder() route: " << route  << " is a ";
    if (stat(route.c_str(), &st) == -1) {
        // Error al obtener información del objeto ¿set errno?
        std::cout << "request isFolder. Error stat" << std::endl;
        return false;
    } else {
        if (S_ISDIR(st.st_mode)) {
            std::cout << "FOLDER" << std::endl;
            return true;
        //if (S_ISREG(st.st_mode)) es fichero regular si ademas tiene permisos es ejecutable && (sb.st_mode & S_IXUSR)
        } else{
            //std::cout << "is NOT a folder" << std::endl;
            if (S_ISREG(st.st_mode)){ // es fichero regular
                std::cout << "regular FILE" << std::endl;
                if (st.st_mode & S_IXUSR)
                    std::cout << "EXECUTABLE file" << std::endl;
             //si ademas tiene permisos es ejecutable && (sb.st_mode & S_IXUSR)
            }
            return false;
        }
    }
}


