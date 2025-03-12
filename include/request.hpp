/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:08 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:09 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <stdbool.h>
#include <sys/stat.h>
#include "fstream"
#include <filesystem>
#include <fstream>
#include <dirent.h>
#include "response.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request{
    protected:
        std::string request_line;
        std::string method;
        std::string uri;
        std::string version;
        std::string URL;
        std::string scheme;
        std::string authority;
        std::string domain;
        std::string path;
        std::string query_string;
        std::string extention;
        std::string resource;

    public:
        std::map<std::string, std::string> header_fields;
        std::string rawRequest;
        request (std::string);
        request ();
        request (request&);
        request& operator=(request&);
        virtual void init() = 0;
        virtual ~request();
        std::string getMethod();
        std::string getUri();
        std::string getRequestLine();
        void parseURL();
        void parseHeader();
        std::string getScheme();
        std::string getPath();
        std::string getExtention();
        std::string getQueryString();
        std::string getDomain();
        std::string getResource();
        void setResource(std::string res);
        void setUri(std::string);
        std::string upFolder(std::string uri);
};
std::vector<std::string> vector_split(const std::string& s, const std::string& delimiter);
