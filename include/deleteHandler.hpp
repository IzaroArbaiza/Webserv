/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:13 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:14 by iarbaiza         ###   ########.fr       */
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
#include "request.hpp"
#include "getRequest.hpp"
#include "struct.hpp"


#define CRLF "\r\n"

class deleteHandler{
    private:
        int _sock;

    public:
        deleteHandler();
        deleteHandler(int);
        void responde(request*, configuration conf);
};
