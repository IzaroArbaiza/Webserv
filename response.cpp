#include "response.hpp"

response::response(){}

response::response(int sock) : socket(sock){

}

response::response(int sock, std::string status) : socket(sock), code(status){

}

response::response(int sock, std::string status, std::string page) : socket(sock), code(status), location(page){

}

response::response(int sock, std::string status, std::map<std::string, std::string> conf) : socket(sock), code(status){
    responseconfig = conf;
    std::cout << "response config created, size: " << responseconfig.size() << std::endl;
}

//response::response(int sock, std::string status, request *entr) : socket(sock), code(status), entry(entr){
//    std::cout <<"response constructor: " << entry->getMethod() << std::endl;
//}

void response::fill_header(){
    std::string content_type;
    std::string content_length = "Content-Length:"; //si va archivo el tamaño
    if (location.substr(location.find(".")) == ".html")
        content_type = "Content-type: text/html";
    if (location.substr(location.find(".")) == ".ico")
        content_type = "Content-Type: image/x-icon";
    if (location.substr(location.find(".")) == ".jpg")
        content_type = "Content-Type: image/jpg";
    if (location.substr(location.find(".")) == ".jpeg")
        content_type = "Content-Type: image/jpeg";
    if (location.substr(location.find(".")) == ".png")
        content_type = "Content-Type: image/png";
    std::string conection = "Connection:Closed";
    std::cout << "response content type: " << content_type << std::endl;
    header.append(content_type);
}

void response::redir_header(std::string location){
    std::string content_type = "Content-type: text/html\r\n";
    header.append(content_type);
    std::string new_location = "Location: ./";
    new_location.append(location);
    header.append(new_location);
    //header.append("Location: ./index.html");
}

void response::fill_body_answer(){
    //contenido de respuesta y conotenido html ¿cómo juntarlo bien?
    //¿boundarys?
}

void response::fill_body(std::string code){
    //contenido de respuesta y conotenido html ¿cómo juntarlo bien?
    //¿boundarys?

    locationPage(code);
    //more actions to more content
}

std::string response::convertOutput(){ //fields to string
    std::string output;

    output.append(status_line);
    output.append("\r\n");
    output.append(header);
    output.append("\r\n\r\n");
    output.append(body);
    return output;
}

void response::sendToClient(std::string response, std::string location){ //response respesta en forma de string, location archivo, pagna html a mandar
    //convertir de campos a string unido(cabecera + \r\n + header_fields + \r\n\r\n + contenido)
    //rutina de mandar string al socket

    std::ifstream resource(location);
    write(this->socket, response.c_str(), response.size());
    while (std::getline(resource, response)) {
        write(this->socket, response.c_str(), response.size());
    }
    resource.close(); // Close the file stream
}

void response::fill_status_line(std::string code){
    std::map<const char*, int> codes;
    codes["200"] = 0; //meter como segundo termido el status line y evitamos el switch
    codes["404"] = 1;
    codes["403"] = 2;
    codes["500"] = 3;
    codes["505"] = 4;
    codes["418"] = 5;
    codes["204"] = 6;
    codes["308"] = 7;
    codes["405"] = 8;
    //include more cases
    
    int i = 0;
    for (std::map<const char*, int>::iterator it = codes.begin(); it != codes.end(); it++)
    {
        i = it->second; //status_line.append(it->second); si pasamos a la versión sin swith std::map<const char*, const char*> codes
        //if (uri.find(it->first) != std::string::npos)
        if(it->first == code)
            break;
    }
    //std::cout << "reponse status case: " << i << std::endl;
    switch(i) {
        case 0:
            status_line.append("HTTP/1.1 200 OK");
            break;
        case 1:
            status_line.append("HTTP/1.1 404 Not Found");
            break;
        case 2:
            status_line.append("HTTP/1.1 403 Forbidden");
            break;
        case 3:
            status_line.append("HTTP/1.1 500 internal server error");
            break;
        case 4:
            status_line.append("HTTP1.1 505 HTTP Version Not Supported");
            break;
        case 5:
            status_line.append("HTTP1.1 418 I'm a teapot. This is not a folder");
            break;
        case 6:
            status_line.append("HTTP/1.1 204 No content");
            break;
        case 7:
            status_line.append("HTTP/1.1 308 Permanent Redirect");
            break;
        case 8:
            status_line.append("HTTP/1.1 405 Method Not Allowed");
            break;
        default:
            // devolver http en todo caso? GET sin uri
            std::cout << "NOT WORKING CASE" << std::endl;
    }
}

void response::locationPage(std::string code){
    std::map<const char*, int> codes;
    codes["200"] = 0;
    codes["404"] = 1;
    codes["403"] = 2;
    codes["500"] = 3;
    codes["418"] = 4;
    codes["204"] = 5;
    codes["405"] = 6;
    codes["308"] = 7;

    //include more cases

    int i = 0;
    for (std::map<const char*, int>::iterator it = codes.begin(); it != codes.end(); it++)
    {
        i = it->second;
        if(it->first == code)
            break;
    }
    //std::cout << "reponse locatin case: " << i << std::endl;
    switch(i) {
        case 0:
            readPage(location);
            break;
        case 1:
            //readPage("404.html");
            readPage(responseconfig["page_404"]);
            break;
        case 2:
            //readPage("403.html");
            readPage(responseconfig["page_403"]);
            break;
        case 3:
            //readPage("500.html");
            readPage(responseconfig["page_500"]);
            break;
        case 4:
            //readPage("418.html");
            readPage(responseconfig["page_418"]);
            break;
        case 5:
            //readPage("delete.html");
            readPage(responseconfig["page_delete"]);
            break;
        case 6:
            readPage(responseconfig["page_405"]);
            break;
        //case 7:
        //    readPage("index.html");
        //    break;
        default:
            // devolver http en todo caso? GET sin uri
            std::cout << "NOT WORKING CASE" << std::endl;
            //return "default";
    }
}

void response::readPage(std::string location){
    std::ifstream resource(location);
    std::string buffer;
    while (std::getline(resource, buffer)) {
        body.append(buffer);
    }
    resource.close();
}

void response::reply(std::string to_body){
    fill_status_line(code); // cambiar para quitar al entrada de code
    fill_header();
    fill_body(code);
    std::string response = convertOutput();
    /*
    for(std::map<std::string, std::string>::iterator it = entry._header.begin(); it != entry._header.end(); it++ ){
        std::cout << "field " << it->first << " value " << it->second << std::endl;
    }*/
    if (to_body != "")
        response.append(to_body);
    //std::cout << "full response: " << std::endl;
    //std::cout << response << std::endl;
    write(this->socket, response.c_str(), response.size());
}

void response::reply(){
    fill_status_line(code); // cambiar para quitar al entrada de code
    if (code == "200")
        fill_header();
    fill_body(code);
    std::string response = convertOutput();
    //std::cout << "full response: " << std::endl;
    //std::cout << response << std::endl;
    write(this->socket, response.c_str(), response.size());
}

void response::redirect(std::string location){
    fill_status_line(code); //manteniendo code podemos distinguir redireccion temporal de permanente
    redir_header(location);
    fill_body(code);
    std::string response = convertOutput();
    write(this->socket, response.c_str(), response.size());
}

//sendError //¿misma rutina de envio pero cambiar la instancia de response? nueva instancia de response?