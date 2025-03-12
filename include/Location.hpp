/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:54:35 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:22:29 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "ServerConf.hpp"
#include <stdexcept>
#include <map>
#include "cserver.hpp"

class Location {
	private:
		std::string	_path;
		std::string _root;
		std::string _index;
		bool		_autoindex;
		std::string _return;
		std::string _alias;
		std::vector<std::string> _cgiPath;
		std::vector<std::string> _cgiExt;
		
	public:
		Location();
		~Location();

		void setLocPath(std::string param);
		void setLocRoot(std::string param);
		void setLocIndex(std::string param);
		void setLocAutoindex(std::string param);
		void setLocReturn(std::string param);
		void setLocAlias(std::string param);
		void setLocCgiPath(std::vector<std::string> path);
		void setLocCgiExt(std::vector<std::string> path);

		std::string	getLocPath();
		std::string	getLocRoot();
		std::string	getLocIndex();
		bool getLocAutoindex();
		std::string	getLocReturn();
		std::string	getLocAlias();
		std::vector<std::string> getLocCgiPath();
		std::vector<std::string> getLocCgiExt();
};

#endif
