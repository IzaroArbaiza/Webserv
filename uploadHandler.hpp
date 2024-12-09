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
#include "parse.hpp"
#include <iostream>
#include "request.hpp"
#include <fstream>
#include <bitset>
#include <sys/wait.h>
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "response.hpp"


class uploadHandler{
    private:
        int _sock;
        std::string file_name;

    public:
    uploadHandler();
    uploadHandler(int);
    void responde(request*);
    std::string fileName(request * entry);
    std::string vblesPost(request * entry);
    std::string fileContent(request * entry);
    void setFileName(std::string);
    std::string getFileName();
};