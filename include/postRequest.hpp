/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:04 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:05 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "request.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class request;

class postRequest : public request{
    private:
        std::string location;
        std::vector<std::string> vbles;
        std::string exec_vbles;
        std::string file_name;
        
    public:
        std::string uri_type;
        std::string request_body;
        std::string boundary;
        std::string file_content;
        postRequest (std::string, configuration);
        postRequest ();
        postRequest (postRequest&);
        postRequest& operator=(postRequest&);
        void init();
        void fillVbles();
        void extractBody();
        void parse_resource(configuration);
        void parseBodyUpload();
        void fillLocation(std::string);
        ~postRequest();
        void uri_extention();
        std::string getVbles();
        std::string getLocation();
        std::string getFileName();
        std::string actionDetector();
        void fillFilename();
        std::string getFileContent();
        void parseUri(configuration & conf);
        void questionUri(configuration & conf);
        void noQuestionUri(configuration & conf);
        bool root_is_location(configuration & conf);
};
