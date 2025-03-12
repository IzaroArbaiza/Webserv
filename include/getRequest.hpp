/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:51 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:52 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "request.hpp"


#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class getRequest : public request{
    private:
        std::string location;
        std::string uri_type;
        std::string vbles;
        std::string file_name;

    public:
        getRequest (std::string, configuration);
        getRequest ();
        getRequest (getRequest&);
        getRequest& operator=(getRequest&);
        void init();
        ~getRequest();
        void uri_extention();
        void parse_location();
        void parse_file();
        void parse_resource(configuration &);
        bool check_index();
        std::string getLocation();
        std::string getUriType();
        std::string getVbles();
        std::string getFileName();
        std::string getResource();
        void parseUri(configuration & conf);
        void questionUri(configuration & conf);
        void noQuestionUri(configuration & conf);
        bool root_is_location(configuration & conf);
        void parseVbles();
};
