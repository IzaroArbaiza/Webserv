/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:27 by iarbaiza          #+#    #+#             */
/*   Updated: 2024/12/19 19:22:44 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "ServerConf.hpp"
#include "../cserver.hpp"

class ServerConf;

class ConfFile {
	private:
		std::string _path;
		std::vector<std::string> _serv;
		size_t	_numServ;

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

		configuration serverParse(std::string &conf, configuration &confis);
		std::vector<std::string> splitParam(std::string conf, std::string delimit);

		template <typename T>
		std::string to_string(const T& value) {
			std::stringstream ss;
			ss << value;
			return ss.str();
		}

		std::string	getPath();
		size_t get_numServ();

		class ThrowException : public std::exception {
			private:
				std::string _msg;
			public:
				ThrowException(const std::string &msg) : _msg(msg) {}
				virtual const char *what() const throw() {
					return (_msg.c_str());
				}
				virtual ~ThrowException() throw() {}
		};
};

#endif