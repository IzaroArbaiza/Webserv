#pragma once
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <bitset>
#include <csignal>
#include <cstdlib>  //exit()
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct locationer {
    std::string location_name;
    std::string location_root;
    std::string index; //page
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
