#include "getRequest.hpp"

getRequest::getRequest(std::string message, configuration config): request(message){
    init();
    (void) config; //parse_resource(config);
    
    //std::cout << "getRequest created" << std::endl;
    //std::cout << "getRequest vbles: " << getVbles() << std::endl;
    //if (!check_index())
    //    std::cout << "The reusorce is a not folder or not index.htm" << std::endl;
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

void getRequest::init(){
    redir_index = false;
    parse_file();
    parse_location();
    parseVbles();
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
    
    /* if (path == ""){ //tratar como redirección!! // redireccionar localhost:8080 a index
        redir_index = true;
        //location = "index.html"; //que se puede acceder a dominio que nos vengan en vez de localhost  dominioenviroment:puerto/index.html
        //resource.append(location);
    }
    else{ */
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
    //}
    //std::cout << "getRequest:: location " << location << std::endl;
}

void getRequest::parse_file(){
    //if (query_string != "")
    //    std::cout << "getRequest parse_file. uri: " << getUri() << std::endl;
    file_name = getQueryString();
    //std::cout << "getRequest parse_file. uri: " << getUri() << std::endl;
    if (file_name.find("file=") != std::string::npos)
        file_name = file_name.substr(file_name.find("file=") + 5);
    else 
        //file_name = file_name.substr(file_name.find("=") + 1, file_name.find("&") - file_name.find("?") - 1);
        file_name = file_name.substr(file_name.find("?") + 1, file_name.find("&") - file_name.find("?") - 1);
    //std::cout << "getRequest parse_file. file: " << file_name << std::endl;
}

void getRequest::parse_resource(configuration config){
    
    resource = config.root;
    //std::string sort_path = extractDirectoryPath(getPath());
    //std::string prueba_ruta = config.root + sort_path;
    //std::cout << "getRequest::parse_resource. sort path: " << sort_path << std::endl;
    if(!path.empty())
        resource = resource + path; // + "/" + static_cast<getRequest *>(entry)->getFileName();
    std::cout << "getRequest::parse_resource. file: " << file_name << std::endl;
    if (!getFileName().empty())
        resource = resource + "/" + getFileName();
    std::cout << "getRequest::parse_resource. resource: " << resource  << std::endl;
}

bool getRequest::check_index(){
    DIR *dir;
    struct dirent *ent;
    std::string path = getResource();
    //std::cout << "resource: " << resource << std::endl << std::endl;
    //std::cout << "CUCU check index : " << path << std::endl;
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
        if (strcmp(ent->d_name, "index.html") == 0){
            closedir(dir);
            return true;
        }
    }
    closedir(dir);
    return false;
}

getRequest::~getRequest(){
}

std::string getRequest::extractDirectoryPath(const std::string& fullPath) {
        // Find the start of query parameters
        size_t queryPos = fullPath.find('?');
        std::string cleanPath = (queryPos != std::string::npos) 
            ? fullPath.substr(0, queryPos) 
            : fullPath;
        
        // Find the last occurrence of '/'
        size_t lastSlashPos = cleanPath.find_last_of('/');
        
        if (lastSlashPos == std::string::npos) {
            // No directory, return root
            return "/";
        }
        
        // Return path up to the last '/', including the trailing slash
        return cleanPath.substr(0, lastSlashPos + 1);
}
