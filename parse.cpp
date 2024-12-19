#include <map>
#include <sstream>
#include <string>
#include "parseo/ServerConf.hpp"

//Cambiar!
/* template <typename T>
std::map<std::string, T> parseGet(std::string& query) {
    std::map<std::string, T> result;
    std::istringstream iss(query);
    std::string key, value;

    while (std::getline(iss, key, '&')) {
        std::getline(iss, value, '=');
        result[key] = static_cast<T>(ServerConf::ft_stoi(value)); // o cualquier otra conversión necesaria
    }

    return result;
} */