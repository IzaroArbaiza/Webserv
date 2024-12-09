/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:04 by iarbaiza          #+#    #+#             */
/*   Updated: 2024/12/02 14:02:27 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVERCONF_HPP
#define SERVERCONF_HPP

#include "ConfFile.hpp"
#include "Location.hpp"
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
		std::vector<Location> _location;
		std::map<int, std::string> _error_page;

	public:
		ServerConf();
		~ServerConf();

		void checkParam(std::string &param);
		bool ipValid(const std::string& ip);
		bool hostValid(const std::string& host);
		void dividePortHost(std::string param);
		void initErrorPages(void);
		bool validErrorPages(void);

		void setPort(std::string param);
		void setHost(std::string param);
		void setServerName(std::string param);
		void setErrorPages(std::vector<std::string> &param);
		void setClientMaxBodySize(std::string param);
		void setRoot(std::string param);
		void setIndex(std::string param);
		void setLocation(std::vector<std::string> param, std::string path);

		int	checkLocValid(Location &loc);
		bool checkFileHtml(std::string root, std::string index);
		bool checkLocations();

		/* const */ int 	getPort();
		const std::string getHost();
		const std::string getServerName();
		/* const */ size_t 	getClientMaxBodySize();
		const std::string getRoot();
		const std::string getIndex();
		std::vector<Location> getLocation();

		int ft_stoi(const std::string& str);
};

#endif