/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:17 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:19 by iarbaiza         ###   ########.fr       */
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
#include "struct.hpp"

class response{
    private:
        std::string status_line;
        std::string header;
        std::string body;
        int socket;
        std::string code;
        std::string location;
    public:
        response();
        response(int);
        response(int, std::string);
        response(int, std::string, std::string);
        response(int, std::string, configuration);
        void fill_status_line(std::string);
        void fill_header();
        void redir_header(std::string);
        void fill_body_answer();
        void fill_body(std::string, configuration conf);
        std::string convertOutput();
        void readPage(std::string);
        void locationPage(std::string code, configuration conf);
        void reply(std::string, configuration conf);
        void reply(configuration);
        void redirect(std::string location, configuration conf);
};
