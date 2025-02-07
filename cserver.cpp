#include "cserver.hpp"
#define ERROR_IN_INPUT             9
#define BACKLOG                   10
#define NUM_FDS                    5

//static void parse_resource(request * entry, configuration conf);

cserver::cserver(){}

//cserver::cserver(configuration  confis) {}

cserver::~cserver(){}

void cserver::dostuff (int sock,configuration & config) //read and answer request from client
{
    std::string client_request;
    client_request = read_client(sock);
    //std::cout << "POLLin in socket: [" << sock <<"]" << std::endl;
    request *entry;
    RequestFactory maker;
    entry = maker.factoryRequest(client_request, config);
    
    configuration conf;
    conf = config;
    is_location(entry, conf);
    
       //if path is a location, the configuration is tunned
    //como resource en entry ya esta creado y parseado, si es location debemos modificarlo para que "surta efecto"
    
   
    if (!client_request.empty() && allowedMethod(sock, entry->getMethod(), conf)){
        if (entry->getMethod() == "POST" && conf.post_allowed == true){ 
        //if (entry->getMethod() == "POST"){ //ya esta checkeado si se usa allowedMethod()
            //std::cout << "server.cpp POST" << std::endl;
            postHandler Handler(sock);
            Handler.responde(entry, conf);
        }
        else if (entry->getMethod() == "DELETE" && conf.delete_allowed == true){ //delete deberia entrarnos como metodo
        //else if (entry->getMethod() == "DELETE"){
            //std::cout << "server.cpp DELETE" << std::endl;
            deleteHandler Handler(sock);
            Handler.responde(entry, conf);
        }
        else if (entry->getMethod() == "GET" && conf.get_allowed == true){
        //else if (entry->getMethod() == "GET"){
            //std::cout << "server.cpp GET" << std::endl;
            getHandler Handler(sock); 
            Handler.responde(entry, conf);
        }
        else{
            response toClient(sock, "405", conf);
            toClient.reply(conf);
        }
    }
    shutdown(sock, 2);
    close(sock);
    delete entry;
}

std::string cserver::read_client (int sock){
    size_t n = 0;
    std::string buffer;
    buffer.resize(50000000); //body size + request line...
    std::string request;

    n = recv(sock, &buffer[0], buffer.size(), 0);
    request.append(buffer);
    if (n == buffer.size()){
        while ( n == buffer.size()){
            n = recv(sock, &buffer[0], buffer.size(), 0);
            if (n > 0)
                request.append(buffer);
        }
    }
    if (n < 0){
        //error("ERROR reading from socket"); //¿como actua un servidor?
        std::cout << "cserver::read_client. ERROR reading from socket" << std::endl;
        shutdown(sock, 2);
    }
    if (n == 0){ //nothing readed, void menssage
        std::cout << "cserver::read_client. ERROR reading from socket" << std::endl;
        shutdown(sock, 2);
    }
    return request;
}

bool cserver::allowedMethod(int sock, std::string method, configuration config){
    if (method == "GET" && config.get_allowed == false){
        std::cout << "GET not allowed" << std::endl;
        std::cout << "because get allowed = " << config.get_allowed << std::endl;
        response toClient(sock, "405");
        toClient.reply(config);
        return false;
    }
    else if (method == "POST" && config.post_allowed == false){
        std::cout << "POST not allowed" << std::endl;
        response toClient(sock, "405", config);
        toClient.reply(config);
        return false;
    }
    else if (method == "DELETE" && config.delete_allowed == false){
        std::cout << "DELETE not allowed" << std::endl;
        response toClient(sock, "405", config);
        toClient.reply(config);
        return false;
    }
    else if(method == "GET" || method == "POST" || method == "DELETE") //metodos soportados
        return true;
    return (false);
}

bool cserver::is_location(request *entry, configuration & conf){
    //std::cout << "cserver::is_location  getResource (entry->resource): " << entry->getResource() << std::endl;
    //std::cout << "cserver::is_location  getUri entry->uri: " << entry->getUri() << std::endl;
    for(int i = 0; i < conf.iter; i++){
        //std::cout << "check locations :" << conf.locations[i].location_name << " - ";
        //SEGFAULT
        //std::cout << "9999 " << conf.locations[i].location_name << " 9999" << std::endl; 
        /* const char* cstr = conf.locations[i].location_name.c_str();
        if (cstr == NULL) {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << cstr << std::endl << std::endl << std::endl << std::endl;
            cstr = "";
        }
        std::string result(cstr); */
        //std::cout << "0000 " << result << " 0000" << std::endl;
        if (entry->getPath() == conf.locations[i].location_name || (entry->getPath().empty() && conf.locations[i].location_name =="/")) {
        //if (conf.locations[i].location_name =="/") {
            //std::cout << "cserver::is_location " << entry->getPath() << std::endl;
            //std::cout << " " << conf.locations[i].location_name;
            change_conf(i, conf);
            return true;
        }
    }
    return false;
}

void cserver::change_conf(int i, configuration & conf){ //server configuration to location configuration
    if(!conf.locations[i].location_root.empty())
        conf.root = conf.locations[i].location_root;
    if(!conf.locations[i].index.empty())
        conf.index = conf.locations[i].index;
    conf.autoindex = conf.locations[i].autoindex;
    conf.get_allowed = conf.locations[i].getOn;
    conf.post_allowed = conf.locations[i].postOn;
    conf.delete_allowed = conf.locations[i].deleteOn;
}

/*
static void parse_resource(request * entry, configuration conf){
    entry->setResource(conf.root + entry->getUri());
    std::cout << "cserver::parse_resource. resource: " << entry->getResource() << std::endl;
}*/
