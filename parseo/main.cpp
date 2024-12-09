#include "Parse.hpp"

//siege -c10 -t1m http://tusitio.com

int main(int argc, char const *argv[])
{
	std::string	cont;
	Parse parse = Parse();

	if (argc == 1 || argc == 2) {
		try {
			if (argc == 1) {
				//std::cout << "Ha llegau un argumento" << std::endl;
				cont = "conf/default.conf";
			} else if (argc == 2) {
				//std::cout << "Han llegau dos argumentos" << std::endl;
				cont = argv[1];
			}
			parse.parse(cont);
		} catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			return (1);
		}
		
	} else {
		std::cout << "Invalid amount of arguments. Try again" << std::endl;
		return (1);
	}
	return (0);
}
