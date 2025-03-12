/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:24 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:25 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <sys/ioctl.h>
#include <csignal>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <string>

struct locationer {
    std::string location_name;
    std::string location_root;
    std::string index;
    std::map<std::string, std::string> extPath;
    bool autoindex;
    bool getOn;
    bool postOn;
    bool deleteOn;
};

typedef struct configuration {
    int         numServs;
    int         iter;
    std::string server_name;
    std::string host;
    std::string port;
    std::string root;
    std::string init_root;
    std::string activeDirectory;
    std::string user;
    std::string index;
    size_t      body_size;
    std::vector<locationer> locations;
    bool get_allowed;
    bool post_allowed;
    bool delete_allowed;
    bool autoindex;
    std::map<int, std::string> errors;
    
    configuration operator=(configuration copy){
        iter = copy.iter;
        numServs = copy.numServs;
        server_name = copy.server_name;
        host = copy.host;
        port = copy.port;
        root = copy.root;
        init_root = copy.init_root;
        activeDirectory = copy.activeDirectory;
        user = copy.user;
        index = copy.index;
        body_size = copy.body_size;
        locations = copy.locations;
        get_allowed = copy.get_allowed;
        post_allowed = copy.post_allowed;
        delete_allowed = copy.delete_allowed;
        autoindex = copy.autoindex;
        errors = copy.errors;
        return *this;
    }
}configuration;
