/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:27:12 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:27:13 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include "request.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "deleteRequest.hpp"
#include "struct.hpp"

#define CRLF "\r\n"
#define FWL "\r\n "

class RequestFactory{
    public:
        std::map<std::string, std::string> requestconfig;
        RequestFactory();
        RequestFactory(configuration);
        RequestFactory(std::string, configuration);
        ~RequestFactory();
        static request * factoryRequest(std::string, configuration);
};
