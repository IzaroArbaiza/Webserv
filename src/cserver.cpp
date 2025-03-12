/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cserver.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:38 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:39 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cserver.hpp"

cserver::cserver(){}
cserver::~cserver(){}

void cserver::dostuff (int sock,configuration & config) {
    std::string client_request;
    client_request = read_client(sock);
    request *entry;
    RequestFactory maker;
    entry = maker.factoryRequest(client_request, config);
    configuration & conf = config;
    is_location(entry, conf);

    if (!client_request.empty() && allowedMethod(sock, entry->getMethod(), conf)){
        if (entry->getMethod() == "POST" && conf.post_allowed == true){
            postHandler Handler(sock);
            Handler.responde(entry, conf);
        } else if (entry->getMethod() == "DELETE" && conf.delete_allowed == true){
            deleteHandler Handler(sock);
            Handler.responde(entry, conf);
        } else if (entry->getMethod() == "GET" && conf.get_allowed == true){
            getHandler Handler(sock); 
            Handler.responde(entry, conf);
        } else{
            response toClient(sock, "405", conf);
            toClient.reply(conf);
        }
    }
    shutdown(sock, 2);
    close(sock);
    delete entry;
}

std::string cserver::read_client (int sock){
    ssize_t n = 0;
    std::string buffer;
    buffer.resize(700000);
    std::string request;
    char buf[7000];
    std::vector<char> message = std::vector<char>();
    do {
        std::memset(buf, 0, sizeof(buf));
        n = recv(sock, buf, sizeof(buf), 0);
        for (ssize_t i = 0; i < n; i++)
            request.push_back(buf[i]);
    } while (n > 0);
    return request;
}

bool cserver::allowedMethod(int sock, std::string method, configuration config){
    if (method == "GET" && config.get_allowed == false){
        response toClient(sock, "405");
        toClient.reply(config);
        return false;
    } else if (method == "POST" && config.post_allowed == false){
        response toClient(sock, "405", config);
        toClient.reply(config);
        return false;
    } else if (method == "DELETE" && config.delete_allowed == false){
        response toClient(sock, "405", config);
        toClient.reply(config);
        return false;
    } else if(method == "GET" || method == "POST" || method == "DELETE")
        return true;
    return (false);
}

bool cserver::is_location(request *entry, configuration & conf){
    for(int i = 0; i < conf.iter; i++){
        if (entry->getPath() == conf.locations[i].location_name || (entry->getPath().empty() && conf.locations[i].location_name =="/")  ){
            change_conf(i, conf);
            return true;
        }
    }
    return false;
}

void cserver::change_conf(int i, configuration & conf){
    if(!conf.locations[i].location_root.empty()){
        conf.root = conf.locations[i].location_root;
        conf.activeDirectory = conf.root;}
    if(!conf.locations[i].index.empty())
        conf.index = conf.locations[i].index;
    conf.autoindex = conf.locations[i].autoindex;
    conf.get_allowed = conf.locations[i].getOn;
    conf.post_allowed = conf.locations[i].postOn;
    conf.delete_allowed = conf.locations[i].deleteOn;
}

