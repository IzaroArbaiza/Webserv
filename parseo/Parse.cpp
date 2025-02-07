#include "Parse.hpp"

Parse::Parse(){}
Parse::~Parse(){}

int Parse::parse(std::string conf, std::vector<configuration> &confis) {
    ConfFile confi(conf);
	std::string content;

    if (confi.checkFile(confi.getPath()) == -1) {
		throw  std::invalid_argument("Non valid file or couldn't open it.");
	}
	if (confi.checkFile(confi.getPath()) == 3) {
		throw  std::invalid_argument("Non valid file.");
	}
	content = confi.readFile(conf);
	if (content.empty()) {
		return (std::cout << "Empty file." << std::endl, 1);
	}
	confi.removeTrash(content);
	confi.splitServers(content);
	confi.checkData(confis);
	return (confi.get_numServ());
}
