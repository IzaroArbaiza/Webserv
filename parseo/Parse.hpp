#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include "ConfFile.hpp"
#include <exception>
#include "../cserver.hpp"

class Parse {
	private:
		
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