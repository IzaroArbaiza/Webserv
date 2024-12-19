#include "getRequest.hpp"

getRequest::getRequest(std::string message): request(message){
    //fill(message);
    initGet();
    std::cout << "getRequest created" << std::endl;
    std::cout << "getRequest vbles: " << getVbles() << std::endl;
    if (!check_index())
        std::cout << "The reusorce is a not folder or not index.htm" << std::endl;
}


getRequest::getRequest(){ std::cout << "getRequest created" << std::endl; parseURL();}

getRequest::getRequest(getRequest& cp){
    *this = cp;
}

getRequest & getRequest::operator=(getRequest& cp){
    (void)cp;
    //actualizar los campos que se copian
    return *this;
}

void getRequest::fill (std::string message){ //sobreesribir
    //el getRequest debe llenr su location y vbles

    std::string header;
    std::string body;
    std::vector<std::string> lines;
    //añadir  el recurso o location que quiere accederse, y la función para obtenerlo
    int pos;
    request_line = message.substr(0, request_line.find(CRLF)); //first line of the message is the Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    //split request line
    method = request_line.substr(0, request_line.find(" "));
    std::cout << "METHOD: " << method << std::endl;
    std::string tmp;
    tmp = message.substr(message.find(" ") + 1, message.find("HTTP") - 5);
    //if (tmp.c_str() != "/")
    if (tmp != "/"){
        uri = tmp;
        std::cout << "URI: -" << uri << "-" << std::endl;
        uri_extention();
    }
    version = request_line.substr(message.find("HTTP"), 8);
    std::cout << "VERSION: " << version << std::endl;
    pos = request_line.find(CRLF) + 1;
    int end = request_line.find("\r\n\r\n");
    header = message.substr(pos, end - pos);
    lines = split(header, CRLF);
    /*
    int i;
    for (i = 1; i < lines.size() - 1; i++){
        if (lines[i] == "\r\n\r\n"){
            i = 0;
            break ;
        }
        _header[lines[i].substr(0,lines[i].find(":"))] = lines[i].substr(lines[i].find(":") + 2,lines[i].find(CRLF));
        //std::cout << "getRequest head fields: " << _header[lines[i].substr(0,lines[i].find(":"))] << std::endl;
    }
    */
    //if (uri.find("?") != std::string::npos)
        parse_file();
    parse_location();
}

void getRequest::initGet(){
    redir_index = false;
    parse_file();
    parse_location();
    parseVbles();
    parse_resource();
}

void getRequest::parseVbles(){
    //parsear las variables que vienen de la query string? si ya vienen en formato, extraerlas solamente
    vbles = query_string.substr(query_string.find_first_of("&") + 1);
}

std::string getRequest::getLocation(){ return location; }

std::string getRequest::getUriType(){ return uri_type; }

std::string getRequest::getVbles(){ return vbles; }

std::string getRequest::getFileName(){ return file_name; }

std::string getRequest::getResource(){ return resource; }

void getRequest::uri_extention(){
    uri_type = uri.substr(uri.find(".") + 1);
    std::cout << "URI extention: " << uri_type << std::endl;
}

void getRequest::parse_location(){
    //std::cout << "getrequest parse_location. domain: " << requestconfig["domain"] << std::endl;
    std::cout << "getrequest parse_location. CUCU" << std::endl;
    
    if (path == ""){ //tratar como redirección!! // redireccionar localhost:8080 a index
        redir_index = true;
        //location = "index.html"; //que se puede acceder a dominio que nos vengan en vez de localhost  dominioenviroment:puerto/index.html
        //resource.append(location);
    }
    else{
        location = path;
        //location = requestconfig["domain"];
        if (query_string != ""){
            location = location.substr(1,location.find('?') - 1);
            //resource = header_fields["Referer"]; //usar localhost??
            //location = domain + path;
            //resource.append(location);
            //resource.append(domain + path + "/" + ressorse);
            //resource.append("/" + file_name);
        }
        else
            location = location.substr(location.find("/") + 1);
    }
    std::cout << "getRequest:: location " << location << std::endl;
}

void getRequest::parse_file(){
    if (query_string != "")
        std::cout << "getRequest parse_file. uri: " << getUri() << std::endl;
    //file_name = getUri();
    file_name = getQueryString();
    std::cout << "getRequest parse_file. uri: " << getUri() << std::endl;
    if (file_name.find("file=") != std::string::npos)
        file_name = file_name.substr(file_name.find("file=") + 5);
    else 
        //file_name = file_name.substr(file_name.find("=") + 1, file_name.find("&") - file_name.find("?") - 1);
        file_name = file_name.substr(file_name.find("?") + 1, file_name.find("&") - file_name.find("?") - 1);
    std::cout << "getRequest parse_file. file: " << file_name << std::endl;
    //std::cout << "getRequest fillFile. ressorse: " << ressorse << std::endl;
}

void getRequest::parse_resource(){
    resource = getcwd(NULL,0);
    //incluir la variable root del config
    resource = resource + "/" + getLocation(); // + "/" + static_cast<getRequest *>(entry)->getFileName();
    if (!getFileName().empty())
        resource = resource + "/" + getFileName();
    std::cout << "request init. resource: " << resource << std::endl;
}

bool getRequest::check_index(){
    DIR *dir;
    struct dirent *ent;
    std::string path = getResource();
    //std::string path = resource;
    std::cout << "CUCU check index : " << path << std::endl;
    dir = opendir(path.c_str());
    bool is = false; 
    
    struct stat st;
    if (stat(path.c_str(), &st) == -1) {
        // Error al obtener información del objeto ¿set errno?
        std::cout << "request isFolder. Error stat" << std::endl;
        is = false;
    }
    if (S_ISDIR(st.st_mode))
        is = true;

    if (!is)
        return false;
        
    if (dir == NULL) {
            // Error al abrir el directorio
    }
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, "index.html") == 0)
            return true;
    }
    closedir(dir);
    return false;
}

getRequest::~getRequest(){
}


//smart contract, tokens 60 horas. mes y medio. gratuito. DNI, recibo autonomos vida laboral y rellenar formulario (ontek)
