#include "response.hpp"

response::response(){}

response::response(int sock) : socket(sock){}

response::response(int sock, std::string status) : socket(sock), code(status){}

response::response(int sock, std::string status, std::string page) : socket(sock), code(status), location(page){}

response::response(int sock, std::string status, configuration conf) : socket(sock), code(status){
    (void) conf;
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

/* void response::fill_body_answer(){
    //contenido de respuesta y conotenido html ¿cómo juntarlo bien?
    //¿boundarys?
} */

void response::fill_body(std::string code, configuration conf){
    //contenido de respuesta y conotenido html ¿cómo juntarlo bien?
    //¿boundarys?

    locationPage(code, conf);
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

/* void response::sendToClient(std::string response, std::string location){ //response respesta en forma de string, location archivo, pagna html a mandar
    //convertir de campos a string unido(cabecera + \r\n + header_fields + \r\n\r\n + contenido)
    //rutina de mandar string al socket

    std::ifstream resource(location);
    write(this->socket, response.c_str(), response.size());
    while (std::getline(resource, response)) {
        write(this->socket, response.c_str(), response.size());
    }
    resource.close(); // Close the file stream
} */

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
            //std::cout << "ging to show page 200" << std::endl;
            break;
        case 1:
            status_line.append("HTTP/1.1 404 Not Found");
            //std::cout << "ging to show page 404" << std::endl;
            break;
        case 2:
            status_line.append("HTTP/1.1 403 Forbidden");
            //std::cout << "ging to show page 403" << std::endl;
            break;
        case 3:
            status_line.append("HTTP/1.1 500 internal server error");
            //std::cout << "ging to show page 500" << std::endl;
            break;
        case 4:
            status_line.append("HTTP1.1 505 HTTP Version Not Supported");
            //std::cout << "ging to show page 505" << std::endl;
            break;
        case 5:
            status_line.append("HTTP1.1 418 I'm a teapot. This is not a folder");
            //std::cout << "ging to show page 418" << std::endl;
            break;
        case 6:
            status_line.append("HTTP/1.1 204 No content");
            //std::cout << "ging to show page 204" << std::endl;
            break;
        case 7:
            status_line.append("HTTP/1.1 308 Permanent Redirect");
            //std::cout << "ging to show page 308" << std::endl;
            break;
        case 8:
            status_line.append("HTTP/1.1 405 Method Not Allowed");
            //std::cout << "ging to show page 405" << std::endl;
            break;
        default:
            // devolver http en todo caso? GET sin uri
            std::cout << "NOT WORKING CASE" << std::endl;
    }
}

void response::locationPage(std::string code, configuration conf) {
    std::map<const char*, int> codes;
    codes["200"] = 0;
    codes["404"] = 1;
    codes["403"] = 2;
    codes["500"] = 3;
    codes["418"] = 4;
    codes["204"] = 5;
    codes["405"] = 6;
    codes["413"] = 7;
    codes["308"] = 8;

    //include more cases
    //std::cout << "---------- " << conf.errors[404] << std::endl;
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
             std::cout << "going to show page " << location << std::endl;
            break;
        case 1:
            //readPage("404.html");
            readPage(conf.errors[404]);
            std::cout << "going to show page " << conf.errors[404] << std::endl;
            break;
        case 2:
            //readPage("403.html");
            readPage(conf.errors[403]);
            std::cout << "going to show page " << conf.errors[403] << std::endl;
            break;
        case 3:
            //readPage("500.html");
            readPage(conf.errors[500]);
            std::cout << "going to show page " << conf.errors[500] << std::endl;
            break;
        case 4:
            //readPage("418.html");
            readPage(conf.errors[418]);
            std::cout << "going to show page " << conf.errors[418] << std::endl;
            break;
        case 5:
            //readPage("204.html");
            readPage(conf.errors[204]);
            std::cout << "going to show page " << conf.errors[204] << std::endl;
            break;
        case 6:
            readPage(conf.errors[405]);
            std::cout << "going to show page " << conf.errors[405] << std::endl;
            break;
        case 7:
            readPage(conf.errors[413]);
            std::cout << "going to show page " << conf.errors[413] << std::endl;
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
    std::ifstream resource(location.c_str());
    std::string buffer;
    while (std::getline(resource, buffer)) {
        body.append(buffer);
    }
    resource.close();
}

void response::reply(std::string to_body, configuration conf){
    fill_status_line(code); // cambiar para quitar al entrada de code
    fill_header();
    fill_body(code, conf);
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

void response::reply(configuration conf){
    fill_status_line(code); // cambiar para quitar al entrada de code
    if (code == "200")
        fill_header();
    fill_body(code, conf);
    std::string response = convertOutput();
    //std::cout << "full response: " << std::endl;
    //std::cout << response << std::endl;
    write(this->socket, response.c_str(), response.size());
}

void response::redirect(std::string location, configuration conf){
    fill_status_line(code); //manteniendo code podemos distinguir redireccion temporal de permanente
    redir_header(location);
    fill_body(code, conf);
    std::string response = convertOutput();
    write(this->socket, response.c_str(), response.size());
}

//sendError //¿misma rutina de envio pero cambiar la instancia de response? nueva instancia de response?
