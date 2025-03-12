/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uploadHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:28 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:29 by iarbaiza         ###   ########.fr       */
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
        void responde(request*, configuration conf);
        std::string fileName(request * entry);
        std::string fileContent(request * entry);
        void setFileName(std::string);
        std::string getFileName();
};
