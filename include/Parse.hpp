/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:26:56 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:26:57 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <exception>
#include "ConfFile.hpp"
#include "cserver.hpp"

class Parse {
	public:
		Parse();
		~Parse();

		int parse(std::string conf, std::vector<configuration> &confis);

		class ErrorException : public std::exception {
			private:
				std::string _msg;
			public:
				ErrorException(const std::string &msg) : _msg(msg) {}
				virtual const char *what() const throw() {
					return (_msg.c_str());
				}
				virtual ~ErrorException() throw() {}
		};
};

#endif
