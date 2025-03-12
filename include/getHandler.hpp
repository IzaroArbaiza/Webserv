/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:45 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:46 by iarbaiza         ###   ########.fr       */
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
#include <stdbool.h>
#include <dirent.h>
#include "request.hpp"
#include "errorHandler.hpp"
#include "response.hpp"
#include "cgiHandler.hpp"
#include "deleteHandler.hpp"

#define CRLF "\r\n"
class cserver;

class getHandler{
    private:
        int _sock;
        std::string resource;

    public:
    getHandler();
    getHandler(int);
    void responde(request * entry, configuration &);
    bool isExecutable(request *, configuration);
    std::string accesable(request * entry);
    void autoindex(request *entry, configuration);
    bool index_in_folder(request *entry, configuration conf);
};
