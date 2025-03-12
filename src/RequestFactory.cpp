/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:19 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:20 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RequestFactory.hpp"

RequestFactory::RequestFactory(configuration config){(void) config;}

RequestFactory::RequestFactory(){}

RequestFactory::RequestFactory(std::string buffer, configuration conf){
    factoryRequest(buffer, conf);
}

RequestFactory::~RequestFactory(){}

request * RequestFactory::factoryRequest(std::string readed, configuration conf){
    std::string tmpRequestLine;
    std::string tmpMethod;
    tmpRequestLine = readed.substr(0, tmpRequestLine.find(CRLF));
    tmpMethod = tmpRequestLine.substr(0, tmpRequestLine.find(" "));
    
    if (tmpMethod == "GET")
        return new getRequest(readed, conf);
    if (tmpMethod == "POST"){
        return new postRequest(readed, conf);
    }
    if (tmpMethod == "DELETE")
      return new deleteRequest(readed, conf);
    return (0);
}
