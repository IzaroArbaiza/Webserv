/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cserver.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:09 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:10 by iarbaiza         ###   ########.fr       */
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
#include <string>
#include <fcntl.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include "struct.hpp"
#include "request.hpp"
#include "getHandler.hpp"
#include "postHandler.hpp"
#include "deleteHandler.hpp"
#include "cgiHandler.hpp"
#include "response.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "RequestFactory.hpp"
#include "Parse.hpp"

#include "struct.hpp"

#define CRLF "\r\n"

class cserver{
    public:
        cserver();
        void dostuff(int, configuration & );
        std::string read_client (int sock);
        bool allowedMethod(int, std::string, configuration);
        bool is_location(request *entry, configuration & config);
        void change_conf(int i, configuration & config);
        ~cserver();  
};
