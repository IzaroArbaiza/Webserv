/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:27 by iarbaiza          #+#    #+#             */
/*   Updated: 2024/12/04 12:28:44 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "ServerConf.hpp"

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
		void		checkData();
		//void		checkServers();
		void		checkServ();

		void serverParse(std::string &conf);
		std::vector<std::string> splitParam(std::string conf, std::string delimit);

		std::string	getPath();

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