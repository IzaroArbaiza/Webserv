#include <unordered_map>
#include <sstream>
#include <string>

template <typename T>
std::unordered_map<std::string, T> parseGet(std::string& query) {
    std::unordered_map<std::string, T> result;
    std::istringstream iss(query);
    std::string key, value;

    while (std::getline(iss, key, '&')) {
        std::getline(iss, value, '=');
        result[key] = static_cast<T>(std::stoi(value)); // o cualquier otra conversión necesaria
    }

    return result;
}