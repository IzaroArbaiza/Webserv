/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:27 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:22:54 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/stat.h> 
#include "ServerConf.hpp"
#include "cserver.hpp"

class ServerConf;

class ConfFile {
	private:
		std::string _path;
		std::vector<std::string> _serv;
		size_t	_numServ;
		std::vector<bool> _methods;

	public:
		ConfFile(const std::string path);
		~ConfFile();

		static int	checkFile(const std::string& path);
		std::string	readFile(std::string file);
		void		removeTrash(std::string &content);
		void		splitServers(std::string &content);
		size_t		startServer(std::string &content, size_t i);
		size_t		endServer(std::string &content, size_t i);
		void		checkData(std::vector<configuration> &confis);
		void		checkServ();
		void	setAllowmethods(std::vector<std::string> meth);
		configuration serverParse(std::string &conf, configuration &confis);
		std::vector<std::string> splitParam(std::string conf, std::string delimit);
		std::string	getPath();
		size_t get_numServ();

		template <typename T>
		std::string to_string(const T& value) {
			std::stringstream ss;
			ss << value;
			return ss.str();
		}
};

#endif
