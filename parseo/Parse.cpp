#include "Parse.hpp"

Parse::Parse(){}
Parse::~Parse(){}

//check the file -> path, readable, permissions...
	// checkFile() for checking the file extension + path + permissions (if cant open, return 1)
//read the file -> check if empty
	// readFile() reads the file (if empty or can't open == NULL), copy the file in content
//remove unnecesary things -> blank lines, comments...
	// removeTrash() -> removes the comments
//split the file
	// splitServers() -> splits enery server and display it in a vector (_serv)
int Parse::parse(std::string conf) {
    ConfFile confi(conf);
	std::string content;

    if (confi.checkFile(confi.getPath()) == -1) {
		return (std::cout << "Non valid path or couldn't open it." << std::endl, 1);
		//throw ErrorException("Non valid file / path or couldn't open it.");
	}
	//std::cout << confi.checkFile(confi.getPath()) << std::endl;
	if (confi.checkFile(confi.getPath()) == 3) {
		return (std::cout << "Non valid file." << std::endl, 1);
		//throw ErrorException("Non valid file / path or couldn't open it.");
	}

	content = confi.readFile(conf);
	if (content.empty()) {
		return (std::cout << "Empty file." << std::endl, 1);
	}

	confi.removeTrash(content);
	confi.splitServers(content);
	//Checking the data and starting the configuration parse of every server
	confi.checkData();
	return (0);
}
