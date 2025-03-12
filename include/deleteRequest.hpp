/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:18 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:19 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "request.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class deleteRequest : public request{
    private:
        std::string location;
        std::string uri_type;
        std::string vbles;
        std::string file_name;

    public:
        deleteRequest (std::string, configuration);
        deleteRequest ();
        deleteRequest (deleteRequest&);
        deleteRequest& operator=(deleteRequest&);
        ~deleteRequest();
        void uri_extention();
        void fillFile();
        void init();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
};
