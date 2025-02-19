#include "postRequest.hpp"

postRequest::postRequest(std::string str, configuration config): request(str) {
    std::cout << "postRequest created" << std::endl;
    init();
    parse_resource(config);
    std::cout << "initialized postRequest" << std::endl;
    std::cout << "    variables (exec_vbles): " << exec_vbles << std::endl;
    std::cout << "    file name: " << file_name << std::endl;
}

postRequest::postRequest(){ std::cout << "postRequest done" << std::endl;}

postRequest::postRequest(postRequest& cp){
    *this = cp;
}

postRequest & postRequest::operator=(postRequest& cp){
    (void)cp;
    //actualizar los campos que se copian
    return *this;
}

void postRequest::init(){
    std::cout << actionDetector() << std::endl;
    boundary = header_fields["Content-Type"].substr(header_fields["Content-Type"].find("boundary=") + 9, header_fields["Content-Type"].find(CRLF));

    extractBody();
    //if (!request_body.empty())
    //    std::cout << "body rellenado" << std::endl;

    //parsear body y ver si es para upload o para cgi ¿hay otras acciones posibles con metodo post?
    if (actionDetector() == "cgi"){
        std::cout << "postRequest iniPost vamos por cgi" << std::endl;
        exec_vbles = request_body.substr(request_body.find_first_of("=") + 1);
        fillFilename();
    }
    if (actionDetector() == "upload"){
        std::cout << "postRequest iniPost vamos por upload" << std::endl;
        std::string boundary = header_fields["Content-Type"].substr(header_fields["Content-Type"].find("boundary=") + 9, header_fields["Content-Type"].find(CRLF));
        std::vector<std::string> body_fields;
        body_fields = split(request_body, "--" + boundary);
        file_content = body_fields[1];
        std::cout << "posrRequest::init file content size " << file_content.size() << std::endl;
    }
    //std::cout << "body: " << request_body << std::endl;
    //std::cout << "POST action: " << actionDetector() << std::endl;
    //std::cout << "location: " << location << std::endl;
    //std::cout << "variables en formato http: " << exec_vbles << std::endl;
}

void postRequest::extractBody(){
    size_t pos = rawRequest.find("\r\n\r\n");
    //size_t end = rawRequest.find(CRLF);
    //request_body = rawRequest.substr(pos, end - pos);
    request_body = rawRequest.substr(pos);
    /*
    //elimiar saltos de linea
    pos = request_body.find(CRLF);
    while (pos != std::string::npos) {
        request_body.erase(pos, 2);
        pos = request_body.find(CRLF);
    }
    */
}

void postRequest::fillVbles(){ //vbles que lleguen en el post
    std::cout << "que tenemos en uri (vble de request) " << uri << std::endl;
}

void postRequest::parseBody(){ //sacar los datos del body
    //std::string file_name;
    std::string file_content;
    std::string file_type;
    std::string file;
    std::string boundary = header_fields["Content-Type"].substr(header_fields["Content-Type"].find("boundary=") + 9, header_fields["Content-Type"].find(CRLF));
    std::vector<std::string> body_fields;
    std::cout << "hay boundary? "<< boundary << std::endl;
    std::cout << "boundary sale parseando: " << header_fields["Content-Type"] << std::endl;
    body_fields = split(request_body, "--" + boundary);
    fillLocation(body_fields[1]);
    std::cout << "postRequest pase body for loop vbles: " << body_fields.size() << std::endl;
    for(size_t i = 2; i < (size_t)body_fields.size(); i++){
        //procesar cada fila, variables. fillVbles
        std::cout << "fila["<<i <<"] " << body_fields[i] << std::endl;
        std::string vble = body_fields[i].substr(body_fields[i].find_first_of("\"") + 1 , body_fields[i].find_last_of("\"") - body_fields[i].find_first_of("\"") -1);
            exec_vbles.append(vble);
            exec_vbles.append("=");
            vble.clear();
            vble = body_fields[i].substr(body_fields[i].find_last_of("\"") + 1, body_fields[i].find_last_of(CRLF) - body_fields[i].find_last_of("\"") + 1);
            exec_vbles.append(vble + "&");
        }
    //Cambiar!
    exec_vbles.erase(exec_vbles.size() - 1);
    std::cout << "postRquest cgi vbles" << exec_vbles << std::endl;
    if (!exec_vbles.empty()){
        size_t pos = exec_vbles.find(CRLF); //elimiar saltos de linea
        while (pos != std::string::npos) {
            exec_vbles.erase(pos, 2);
            pos = exec_vbles.find(CRLF);
        }
    }
}

void postRequest::parse_resource(configuration config){
    //resource = getcwd(NULL,0);
    resource = config.root;
    //resource = resource + "/" + getLocation();
    if (!getFileName().empty())
        resource = resource + "/" + getFileName();
    std::cout << "postRequest::parse_resource. resource: " << resource << std::endl;
}

void postRequest::parseBodyUpload(){

}

std::string postRequest::getLocation(){
    return location;
}

void postRequest::fillLocation(std::string str){
    size_t from;
    size_t end;
    //std::cout << "post request fillLocation entrada: " << str << std::endl;
    from = str.find("filename=\"") + 10;
    end = str.find("\"", from);
    file_name = str.substr( from, end - from);
    location = header_fields["Referer"].append(file_name); //Referer deberia ser localhost?
}

void postRequest::fillFilename(){
    file_name = request_body.substr(request_body.find_first_of("=") + 1, request_body.find_first_of("&") - request_body.find_first_of("=") - 1);
    size_t pos = file_name.find(CRLF); //elimiar saltos de linea
    while (pos != std::string::npos) {
        file_name.erase(pos, 2);
        pos = file_name.find(CRLF);
    }
}

void postRequest::uri_extention(){
    uri_type = uri.substr(uri.find(".") + 1);
    std::cout << "URI extention: " << uri_type << std::endl;
}

postRequest::~postRequest(){

}

std::string postRequest::getVbles(){
    return exec_vbles;
}

std::string postRequest::getFileName(){
    return file_name;
}

std::string postRequest::getFileContent(){
    return file_content;
}

std::string postRequest::actionDetector()
{
    std::string type = header_fields["Content-Type"];
    
    if (type.find("multipart/form-data") != std::string::npos)
        return "upload";
    else if (type.find("application/x-www-form-urlencoded") != std::string::npos)
        return "cgi";
    return (0);
}
