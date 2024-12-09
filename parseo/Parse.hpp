#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include "ConfFile.hpp"
#include <exception>

class Parse {
	private:
		
	public:
		Parse();
		~Parse();

		int parse(std::string conf);

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