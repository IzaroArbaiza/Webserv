/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:04 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/02/07 12:14:20 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVERCONF_HPP
#define SERVERCONF_HPP

#include "ConfFile.hpp"
#include "Location.hpp"
#include "../cserver.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>

class Location;
class ServerConf {
	private:
		std::string	_host;
		int			_port;
		std::string	_serverName;
		size_t		_clientMaxBodySize;
		std::string	_root;
		std::string	_index;
		std::vector<bool> _methods;
		std::vector<Location> _location;
		std::map<int, std::string> _error_page;

	public:
		ServerConf();
		~ServerConf();

		void checkParam(std::string &param);
		bool ipValid(const std::string& ip);
		bool hostValid(const std::string& host);
		void dividePortHost(std::string param);
		bool validErrorPages(void);

		void setPort(std::string param);
		void setHost(std::string param);
		void setServerName(std::string param);
		void setErrorPages(std::vector<std::string> &param, configuration &confis);
		void setClientMaxBodySize(std::string param);
		void setRoot(std::string param);
		void setIndex(std::string param);
		void setLocAllowmethods(std::vector<std::string> meth);
		locationer setLocation(std::vector<std::string> param, std::string path, locationer loc1);

		int	checkLocValid(Location &loc);
		bool checkFileHtml(std::string root, std::string index);
		bool checkLocations();
		int ft_stoi(const std::string& str);

		/* const */ int 	getPort();
		const std::string getHost();
		const std::string getServerName();
		/* const */ size_t 	getClientMaxBodySize();
		const std::string getRoot();
		const std::string getIndex();
		std::vector<Location> getLocation();
};

#endif
