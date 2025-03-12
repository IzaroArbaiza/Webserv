/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:00:20 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:24:37 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerConf.hpp"

ServerConf::ServerConf() {
	_port = 0;
	_clientMaxBodySize = 0;
	initErrorPages();
}
ServerConf::~ServerConf() {}

void ServerConf::checkParam(std::string &param) {
	size_t pos;

	pos = param.rfind(';');
	if (pos != param.size()-1) {
		throw(std::invalid_argument("No valid parameter"));
	}
	param.erase(pos);
}

bool ServerConf::ipValid(const std::string& ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}

bool ServerConf::hostValid(const std::string& host) {
    if (ipValid(host)) {
        return true;
    }
    struct hostent *he = gethostbyname(host.c_str());
    if (he) {
        return true;
    } else {
        return false;
    }
}

void ServerConf::dividePortHost(std::string param) {
	size_t pos;
	std::string host;
	std::string port;

	checkParam(param);
	pos = param.find(':');
	if (pos < param.length()) {
		host = param.substr(0, pos);
		port = param.substr(pos+1);	
	}
	if (!getHost().empty()){
		throw(std::invalid_argument("Host is duplicated"));
	}
	if (getPort() != 0){
		throw(std::invalid_argument("Port is duplicated"));
	}
	setHost(host);
	setPort(port);
	
}

void ServerConf::initErrorPages(void) {
	this->_error_page[404] = "";
	this->_error_page[418] = "";
	this->_error_page[500] = "";
}

bool ServerConf::validErrorPages(void) {
	std::map<int, std::string>::const_iterator it;

	for (it = this->_error_page.begin(); it != this->_error_page.end(); it++) {
		if (it->first < 400 || it->first > 511) {
			return (false);
		}
		if (ConfFile::checkFile(getRoot() + it->second) < 0) {
			return (false);
		}
	}
	return (true);
}

bool ServerConf::checkFileHtml(std::string root, std::string index) {
	if (ConfFile::checkFile(index) == 1 && ConfFile::checkFile(root) == 1) {
		return (true);
	}
	if (ConfFile::checkFile(root + index) == 1) {
		return (true);
	}
	return (false);
}

bool ServerConf::checkLocations() {
	if (this->_location.size() < 2) {
		return (false);
	}
	return (false);
}

int ServerConf::ft_stoi(const std::string& str) {
    if (str.empty() || str.length() > 10) {
        throw(std::invalid_argument("Non valid string."));
    }
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
			throw(std::invalid_argument("Input contains non-digit characters."));
        }
    }
    int res = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        res = res * 10 + (str[i] - '0');
        if (res < 0) {
			throw(std::invalid_argument("Number out of range."));
        }
    }
    return (res);
}

void ServerConf::setHost(std::string param) {
	if (param == "localhost") {
		param = "127.0.0.1";
	}

	if (!hostValid(param)) {
		throw(std::invalid_argument("Wrong Host"));
	}
	this->_host = param;
}

void ServerConf::setPort(std::string param) {
	int port;
	
	for (size_t i = 0; i < param.length(); i++) {
		if (!std::isdigit(param[i])) {
			throw(std::invalid_argument("Wrong Port: contains non-digit characters."));
		}
	}
	port = ft_stoi(param);
	if (port < 1 || port > 65536) {
		throw(std::invalid_argument("Wrong Port: is not a valid port."));
	}
	this->_port = (int)port;
}

void ServerConf::setServerName(std::string param) {
	checkParam(param);
	this->_serverName = param;
}

void ServerConf::setErrorPages(std::vector<std::string> &param, configuration &confis) {
	if (param.empty()) {
		return ;
	}
	if (param.size() % 2 != 0) {
		throw std::runtime_error("Wrong code: Something went wrong.");
	}
	for (size_t i = 0; i < param.size()-1; i++) {		
		for (size_t j = 0; j < param[i].size(); j++) {
			if (!std::isdigit(param[i][j])) {
				throw(std::invalid_argument("Wrong code: contains non-digit characters."));
			}
		}
		if (param[i].size() != 3) {
			throw(std::invalid_argument("Wrong code: is no valid."));
		}
		int errors = ft_stoi(param[i]);
		if (errors < 400 || errors > 520) {
			throw std::invalid_argument("Wrong error code.");
		}
		i++;
		std::string path = param[i];
		checkParam(path);
		path = this->_root + path;
		if (ConfFile::checkFile(path) != 2) {
			if (ConfFile::checkFile(path) == -1) {
				throw std::invalid_argument("Error page: Incorrect path");
			}
			if (ConfFile::checkFile(path) == -1) {
				throw std::invalid_argument("Error page: Couldn't access");
			}
		}
		std::map<int, std::string>::iterator it = this->_error_page.find(errors);
		std::map<int, std::string>::iterator it2 = confis.errors.find(errors);
		if (it != this->_error_page.end() && it2 != confis.errors.end()) {
			this->_error_page[errors] = path;
			confis.errors[errors] = path;
		} else {
			this->_error_page.insert(std::make_pair(errors, path));
			confis.errors.insert(std::make_pair(errors, path));
		}
	}
}

void ServerConf::setClientMaxBodySize(std::string param) {
	size_t body = 0;

	checkParam(param);
	for (size_t i = 0; i < param.length(); i++) {
		if (!std::isdigit(param[i])) {
			throw(std::invalid_argument("Wrong body size: contains contains non-digit characters."));
		}
	}
	if (!ft_stoi(param)) {
		throw(std::invalid_argument("Wrong body size: Non valid number."));
	}
	body = ft_stoi(param);
	this->_clientMaxBodySize = body;
}

void ServerConf::setRoot(std::string param) {
	char dir[1024];
	std::string root;

	checkParam(param);
	if (ConfFile::checkFile(param) == 2) {
		this->_root = param;
		return ;
	}
	if (getcwd(dir, 1024) == NULL) {
		throw(std::invalid_argument("Wrong root: Couldn't get directory."));
	}
	root = std::string(dir) + param;
	if (ConfFile::checkFile(root) != 2) {
		throw(std::invalid_argument("Wrong root: Couldn't get directory."));
	}
	this->_root = root;
}

void ServerConf::setIndex(std::string param) {
	checkParam(param);
	this->_index = param;
}

locationer ServerConf::setLocation(std::vector<std::string> param, std::string path, locationer loc1) {
	Location loc;
	bool	autoin = false;
	bool	allowmeth = false;

	loc.setLocPath(path);
	for (size_t i = 0; i < param.size(); i++) {
		if (param[i] == "root" && (i+1) < param.size()) {
			if (!loc.getLocRoot().empty()) {
				throw(std::invalid_argument("Loc Error: Root is duplicated."));
			}
			i += 1;
			checkParam(param[i]);
			if (ConfFile::checkFile(param[i]) == 2) {
				loc.setLocRoot(param[i]);
			} else {
				loc.setLocRoot(this->_root + param[i]);
			}
		} else if (param[i] == "index" && (i+1) < param.size()) {
			if (!loc.getLocIndex().empty()) {
				throw(std::invalid_argument("Loc Error: Index is duplicated."));
			}
			i += 1;
			checkParam(param[i]);
			loc.setLocIndex(param[i]);
		} else if (param[i] == "autoindex" && (i+1) < param.size()) {
			if (path == "/cgi-bin") {
				throw(std::invalid_argument("Loc Error: Autoindex not allowed for CGI."));
			}
			if (autoin) {
				throw(std::invalid_argument("Loc Error: Autoindex is duplicated."));
			}
			i += 1;
			checkParam(param[i]);
			loc.setLocAutoindex(param[i]);
			autoin = true;
		} else if (param[i] == "allow_methods" && (i+1) < param.size()) {
			std::vector<std::string> meth;
			
			if (allowmeth) {
				throw(std::invalid_argument("Loc Error: Allowed methods are duplicated."));
			}
			while (++i < param.size()) {
				if (param[i].find(";") != std::string::npos) {
					checkParam(param[i]);
					meth.push_back(param[i]);
					break;
				} else {
					meth.push_back(param[i]);
					if (i+1 >= param.size()) {
						throw(std::invalid_argument("Loc Error: Allowed methods: invalid parameter."));
					}
				}
			}
			setLocAllowmethods(meth);
			allowmeth = true;
		} else if (param[i] == "return" && (i+1) < param.size()) {
			if (!loc.getLocReturn().empty()) {
				throw(std::invalid_argument("Loc Error: Return is duplicated."));
			}
			i += 1;
			checkParam(param[i]);
			loc.setLocReturn(param[i]);
		} else if (param[i] == "alias" && (i+1) < param.size()) {
			if (path == "/cgi-bin") {
				throw(std::invalid_argument("Loc Error: Invalid alias path."));
			}
			if (!loc.getLocAlias().empty()) {
				throw(std::invalid_argument("Loc Error: Alias is duplicated."));
			}
			i += 1;
			checkParam(param[i]);
			loc.setLocAlias(param[i]);
		} else if (param[i] == "cgi_path" && (i+1) < param.size()) {
			std::vector<std::string> path;
			while (++i < param.size()) {
				if (param[i].find(";") != std::string::npos) {
					checkParam(param[i]);
					path.push_back(param[i]);
					break ;
				} else {
					path.push_back(param[i]);
					if (i + 1 >= param.size()) {
						throw(std::invalid_argument("Loc Error: Invalid paramether."));
					}
				}
				if (param[i].find("/python3") == std::string::npos && param[i].find("/bash") == std::string::npos) {
					throw std::invalid_argument("Loc Error: Invalid cgi_path");
				}
			}
			loc.setLocCgiPath(path);
		} else if (param[i] == "cgi_ext" && (i+1) < param.size()) {
			std::vector<std::string> ext;

			while (i+1 < param.size()) {
				i++;
				if (param[i].find(";") != std::string::npos) {
					checkParam(param[i]);
					ext.push_back(param[i]);
					break ;
				} else {
					ext.push_back(param[i]);
					if (i + 1 >= param.size()) {
						throw(std::invalid_argument("Loc Error: Invalid paramether."));
					}
				}
			}
			loc.setLocCgiExt(ext);
		} else if (i < param.size()) {
			throw (std::invalid_argument("Loc Error: Wrong paramether."));
		}
	}
	if (loc.getLocIndex().empty() && loc.getLocPath() != "/cgi-bin") {
		loc.setLocIndex(this->_index);
	}
	int valid = checkLocValid(loc);
	switch (valid) {
		case 1:
			throw std::logic_error("Failed Validating Cgi");
		case 2:
			throw std::logic_error("Failed Validating Path");
		case 3:
			throw std::logic_error("Failed Validating Return");
		case 4:
			throw std::logic_error("Failed Validating Alias");
		default:
			break ;
	}
	this->_location.push_back(loc);

	configuration conf1;
	
	loc1.index = loc.getLocIndex();
	loc1.location_root = loc.getLocRoot();
	loc1.autoindex = loc.getLocAutoindex();
	loc1.deleteOn = false;
	loc1.postOn = false;
	loc1.getOn = false;
	for (size_t i = 0; i < this->_methods.size(); i++) {
		if (this->_methods[0] == true) {
			loc1.getOn = true;
		}
		if (this->_methods[1] == true) {
			loc1.postOn = true;
		}
		if (this->_methods[2] == true) {
			loc1.deleteOn = true;
		} else {
			break ;
		}
	}
	loc1.extPath.insert(_extPath.begin(), _extPath.end());
	return (loc1);	
}

void	ServerConf::setLocAllowmethods(std::vector<std::string> meth) {
	this->_methods = std::vector<bool>(3, false);
	
	for (size_t i = 0; i < meth.size(); i++) {
		if (meth[i] == "GET") {
			this->_methods[0] = true;
		} else if (meth[i] == "POST") {
			this->_methods[1] = true;
		} else if (meth[i] == "DELETE") {
			this->_methods[2] = true;
		} else {
			throw(std::invalid_argument("Error: No valid method in location."));
		}
	}
}

int ServerConf::checkLocValid(Location &loc) {
	std::vector<std::string>::const_iterator it;
	std::vector<std::string>::const_iterator it_path;

	if (loc.getLocPath() == "/cgi-bin") {	
		if (loc.getLocCgiPath().empty() || loc.getLocCgiExt().empty() || loc.getLocIndex().empty()) {
			return (1);
		}
		if (ConfFile::checkFile(loc.getLocIndex()) < 0) {
			std::string path = loc.getLocRoot() + loc.getLocPath() + "/" + loc.getLocIndex();
			
			if (ConfFile::checkFile(path) != 1) {
				path = loc.getLocRoot() + loc.getLocPath() + "/" + loc.getLocIndex();
				if (loc.getLocRoot() == "./") {
					path = getRoot() + loc.getLocPath() + "/" + loc.getLocIndex();
				}
			} 
			if (path.empty() || ConfFile::checkFile(path) == -1) {
				return (1);
			}
		}
		if (loc.getLocCgiPath().size() != loc.getLocCgiExt().size()) {
			return (1);
		}
		const std::vector<std::string> &cgiPaths = loc.getLocCgiPath();
		for (it = cgiPaths.begin(); it != cgiPaths.end(); ++it) {
			if (ConfFile::checkFile(*it) < 0) {
				return (1);
			}
		}
		const std::vector<std::string> &cgiExc = loc.getLocCgiExt();
		for (it = cgiExc.begin(); it != cgiExc.end(); ++it) {
			std::string tmp = *it;
			if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh") {
				return (1);
			}
			const std::vector<std::string> &cgiExc_path = loc.getLocCgiPath();
			for (it_path = cgiExc_path.begin(); it_path != cgiExc_path.end(); ++it_path) {
				std::string tmp_path = *it_path;
				if (tmp == ".py" || tmp == "*.py") {
					if (tmp_path.find("python") != std::string::npos) {
						this->_extPath[".py"] = tmp_path;
					}
				} else if (tmp == ".sh" || tmp == "*.sh") {
					if (tmp_path.find("bash") != std::string::npos) {
						this->_extPath[".sh"] = tmp_path;
					}
				}
			}
		}
		if (loc.getLocCgiPath().size() != getLocExtPath().size()) {
			return (1);
		}
	} else { 
		if (loc.getLocPath()[0] != '/') {
			return (2);
		}
		if (loc.getLocRoot().empty()) {
			loc.setLocRoot(this->_root);
		}
		if (!loc.getLocReturn().empty()) {
			if (checkFileHtml(loc.getLocRoot(), loc.getLocReturn())) {
				return (3);
			}
		}
		if (!loc.getLocAlias().empty()) {
			if (checkFileHtml(loc.getLocRoot(), loc.getLocAlias())) {
				return (4);
			}
		}
		if (checkFileHtml(loc.getLocRoot() + loc.getLocPath() + "/", loc.getLocIndex())) {
			return (5);
		}
	}
	return (0);
}

std::map<std::string, std::string> &ServerConf::getLocExtPath() {
	return (this->_extPath);
}

int	ServerConf::getPort() {
	return (this->_port);
}

const std::string	ServerConf::getHost() {
	return (this->_host);
}

const std::string ServerConf::getServerName() {
	return (this->_serverName);
}

size_t ServerConf::getClientMaxBodySize() {
	return (this->_clientMaxBodySize);
}

const std::string ServerConf::getRoot() {
	return (this->_root);
}

const std::string ServerConf::getIndex() {
	return (this->_index);
}

std::vector<Location> ServerConf::getLocation() {
	return (this->_location);
}
