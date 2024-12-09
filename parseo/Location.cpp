/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:54:37 by iarbaiza          #+#    #+#             */
/*   Updated: 2024/12/03 13:57:40 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

void	Location::setLocPath(std::string param) {
	this->_path = param;
}

void	Location::setLocRoot(std::string param) {
	if (ConfFile::checkFile(param) != 2) {
		throw(std::invalid_argument("Error: no valid root for location"));
	}
	this->_root = param;
}

void	Location::setLocIndex(std::string param) {
	this->_index = param;
}

void	Location::setLocAutoindex(std::string param) {
	if (param == "on") {
		this->_autoindex = true;
	} else if (param == "off") {
		this->_autoindex = false;
	} else {
		throw(std::invalid_argument("Error: wrong autoindex"));
	}
}

void	Location::setLocAllowmethods(std::vector<std::string> meth) {
	this->_methods = std::vector<bool>(3, false);
	
	for (size_t i = 0; i < meth.size(); i++) {
		if (meth[i] == "GET") {
			this->_methods[0] = true;
		} else if (meth[i] == "POST") {
			this->_methods[1] = true;
		} else if (meth[i] == "DELETE") {
			this->_methods[2] = true;
		} else {
			throw(std::invalid_argument("Error: No valid method."));
		}
	}
}

void Location::setLocReturn(std::string param) {
	this->_return = param;
}

void Location::setLocAlias(std::string param) {
	this->_alias = param;
}

void Location::setLocCgiPath(std::vector<std::string> path) {
	this->_cgiPath = path;
}

void Location::setLocCgiExt(std::vector<std::string> ext) {
	this->_cgiExt = ext;
}




std::string Location::getLocPath() {
	return (this->_path);
}

std::string Location::getLocRoot() {
	return (this->_root);
}

std::string Location::getLocIndex() {
	return (this->_index);
}

bool Location::getLocAutoindex() {
	return (this->_autoindex);
}

std::vector<bool> Location::getLocAllowmethods() {
	return (this->_methods);
}

std::string Location::getLocReturn() {
	return (this->_return);
}

std::string Location::getLocAlias() {
	return (this->_alias);
}

std::vector<std::string> Location::getLocCgiPath() {
	return (this->_cgiPath);
}

std::vector<std::string> Location::getLocCgiExt() {
	return (this->_cgiExt);
}

std::map<std::string, std::string> &Location::getLocExtPath() {
	return (this->_extPath);
}
