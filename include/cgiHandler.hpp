/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:01 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:04 by iarbaiza         ###   ########.fr       */
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
#include "struct.hpp"
#include <iostream>
#include "request.hpp"
#include <fstream>
#include <bitset>
#include <sys/wait.h>
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "response.hpp"
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.hpp"


class cgiHandler{
    private:
        int _sock;
        std::string resource;
    public:
        cgiHandler();
        cgiHandler(int);
        void responde(request*, configuration conf);
        void execBinary(std::string file_name, request*, configuration);
        void execPy(std::string file_name, request*, configuration, std::string);
        std::string cgiFileName(request * entry);
        std::string vblesMethod(request * entry);
        std::string resourceMethod(request *entry);
        void to_location_resource(request * entry, configuration);
};
