#include "cserver.hpp"
#define SERVER_PORT                "8080" //viene de enviroment
#define ERROR_IN_INPUT             9
#define BACKLOG                   10
#define NUM_FDS                    5

cserver::cserver(){
    init_server();
}

/*
cserver::cserver(valores){
    //init_server();
}*/

void cserver::init_server(){
    //get config values and init server vbles
    //lanzar desde aqui el parseo de config o desde el main?

    //port
    //root
    //error pages
    //allowed methods

    //default config
    config["port"] = "8080";
    config["location"] = "localhost"; //?
    config["root"] = "/home/";
    std::string user = getenv("USER");
    config["root"].append(user);
    config["root"].append(getcwd(NULL,0));
    std::cout << "init server defalut root: " << config["root"] << std::endl;
    config["page_404"] = "404.html";
    config["page_403"] = "403.html";
    config["page_405"] = "405.html";
    config["page_418"] = "418.html";
    config["page_500"] = "Alt500.html";
    config["page_delete"] = "delete.html";
    get_allowed = true;
    post_allowed = true;
    delete_allowed = true;
}

cserver::~cserver(){}

void cserver::dostuff (int sock) //read and answer request from client
{
    std::string client_request;
    client_request = read_client(sock);
    std::cout << "POLLin in socket: [" << sock <<"]" << std::endl;
    request *entry;
    RequestFactory maker(config);
    entry = maker.factoryRequest(client_request);
    std::cout << "cserver. request line: " << entry->getRequestLine() << std::endl;
    if (!client_request.empty() && allowedMethod(sock, entry->getMethod(), config)){
    //if (!client_request.empty()){ //without checing allowed method
        std::cout << "request method: " << entry->getMethod() << std::endl;
        //if (entry->getMethod() == "POST" && post_allowed){ 
        if (entry->getMethod() == "POST"){ //ya esta checkeado si es allowed
            std::cout << "server.cpp POST" << std::endl;
            postHandler Handler(sock);
            Handler.responde(entry);
        }
        //else if (entry->getMethod() == "DELETE" && delete_allowed){ //delete deberia entrarnos como metodo
        else if (entry->getMethod() == "DELETE"){
            std::cout << "server.cpp DELETE" << std::endl;
            deleteHandler Handler(sock);
            Handler.responde(entry);
        }
        //else if (entry->getMethod() == "GET" && get_allowed){
        else if (entry->getMethod() == "GET"){
            std::cout << "server.cpp GET" << std::endl;
            getHandler Handler(sock, config);
            Handler.responde(entry);
        }
    }
    //buffer.clear();
    shutdown(sock, 2);
    close(sock);
    delete entry;
}

std::string cserver::read_client (int sock){
    int n = 0;
    std::string buffer;
    buffer.resize(50000000);
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
    if (n < 0)
        //error("ERROR reading from socket"); //¿como actua un servidor?
        shutdown(sock, 2);
    if (n == 0) //nothing readed, void menssage
        shutdown(sock, 2);
    return request;
}

bool cserver::allowedMethod(int sock, std::string method, std::map<std::string, std::string> config){
    if (method == "GET" && !get_allowed){
        std::cout << "GET not allowed" << std::endl;
        response toClient(sock, "405", config);
        toClient.reply();
        return false;
    }
    else if (method == "POST" && !post_allowed){
        std::cout << "POST not allowed" << std::endl;
        response toClient(sock, "405", config);
        toClient.reply();
        return false;
    }
    else if (method == "DELETE" && !delete_allowed){
        std::cout << "DELETE not allowed" << std::endl;
        response toClient(sock, "405", config);
        toClient.reply();
        return false;
    }
    else if(method == "GET" || method == "POST" || method == "DELETE") //metodos soportados
        return true;
}
