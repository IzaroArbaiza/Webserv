#include <unordered_map>
#include <sstream>
#include <string>

template <typename T>
std::unordered_map<std::string, T> parseGet(std::string& query);