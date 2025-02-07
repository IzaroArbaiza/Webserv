#include "cserver.hpp"
#define BACKLOG 10
#define MAX_CLIENTS 1000


void manejar_signal(int sig);
void error(/*const*/ std::string msg);
int parse_config(int argc, char const * argv[], std::vector<configuration> &confis);
void fill_config(int n, std::vector<configuration> &confis);
int socket_listening(std::vector<configuration>& configs);

void manejar_signal(int sig) {
    (void)sig;
    std::cout << "Recibida señal SIGINT (Ctrl+C), cerrando servidor..." << std::endl;
    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, manejar_signal);
    int server_amount = 0;
    std::vector<configuration > confis;
    server_amount = parse_config(argc, argv, confis);
    if (server_amount > 0) {
        fill_config(server_amount , confis);
        socket_listening( confis);
    }
    return (0);
}

void error(/*const*/ std::string msg) //actualiza error y sale del bucle, cierra el server en c++ pasar a exception
{
    std::perror (msg.c_str());
    exit (1);
}

int parse_config(int argc, char const * argv[], std::vector<configuration> &confis) {
	std::string	cont;
    int vectorsize;
    
	Parse parse = Parse();
	if (argc == 1 || argc == 2) {
		try {
			if (argc == 1) {
				cont = "conf/default.conf";
			} else if (argc == 2) {
				cont = argv[1];
			}
			vectorsize = parse.parse(cont, confis);

		} catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			//response toClient(_sock, "500");
            //toClient.reply();
			return (-1);
		}
	} else {
		std::cout << "Invalid amount of arguments. Try again" << std::endl;
        return (-1);
	}
    std::cout << "main::parse_config is GET allowed? " << confis[0].get_allowed << std::endl;
    return (vectorsize);
}

std::string to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

void htmlButtom(configuration &confis, std::string &fileName) {
    std::ifstream entrada(fileName.c_str());
    std::string content;
    std::string line;

    while (std::getline(entrada, line)) {
        content += line + "\n";
    }
    entrada.close();

    std::string oldButtomStart = "<button onclick=\"location.href='";
    std::string oldButtomEnd = "'\">to Index</button>";

    std::string newButtomStart = "<button onclick=\"location.href='";
    std::string newButtomEnd = "'\">to Index</button>";

    size_t startPos = 0;
    while ((startPos = content.find(oldButtomStart, startPos)) != std::string::npos) {
        size_t endPos = content.find(oldButtomEnd, startPos);
        if (endPos != std::string::npos) {
            std::string oldHref = content.substr(startPos + oldButtomStart.length(), endPos - (startPos + oldButtomStart.length()));
            std::string newHref = confis.index;
            std::string newButtom = newButtomStart + newHref + newButtomEnd;

            content.replace(startPos, endPos + oldButtomEnd.length() - startPos, newButtom);
            startPos += newButtom.length();
        }
    }
    std::ofstream salida(fileName.c_str(), std::ios::trunc);
    salida << content;
    salida.close();
}

void agregarErroresPredeterminados(std::map<int, std::string> &mapaError) {
    int errors[] = {200, 204, 308, 403, 404, 405, 413, 418, 500};

    for (size_t i = 0; i < sizeof(errors)/sizeof(errors[0]); ++i) {
        int codigoError = errors[i];

        if (mapaError.find(codigoError) == mapaError.end()) {
            mapaError[codigoError] = "error_pages/" + to_string(codigoError) + ".html";
        }
    }
}

void fill_config(int n, std::vector<configuration> &confis) {
    //default struct configuration
    for (int i = 0; i < n; i++) {
        std::string rut = "/home/";
        confis.at(i).user = getenv("USER");
        rut.append(confis.at(i).user);

        //Parte añadida para liberar los 51 bytes
        char* cwd = getcwd(NULL, 0);    //Asignación de memoria dinámica
        if (cwd == NULL) {
            //Manejo de error si getcwd falla
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
        rut.append(cwd);    //Añadir el directorio actual a la ruta
        free(cwd);

        if (confis.at(i).numServs != 1)
            confis.at(i).numServs = 1;
        if (confis.at(i).server_name.empty())
            confis.at(i).server_name = "localhost";
        if (confis.at(i).host.empty())
            confis.at(i).host = "127.0.0.1";
        if (confis.at(i).port.empty())
            confis.at(i).port = "8080";
        if (confis.at(i).root.empty())
            confis.at(i).root = rut;
        if (confis.at(i).index.empty())
            confis.at(i).index = "./index";
        if (confis.at(i).body_size <= 0)
            confis.at(i).body_size = 1024;
        agregarErroresPredeterminados(confis.at(i).errors);
        for (int j = 0; j < (int)confis.at(i).errors.size(); j++) {
            htmlButtom(confis.at(i), confis.at(i).errors[j]);
        }
        //agregarErroresPredeterminados(confis.at(i).errors);
        //std::string rute = "error_pages/404.html";
        //htmlButtom(confis.at(i), rute);
        //agregarErroresPredeterminados(confis.at(i).errors);
    }
}

// Helper function to find in vector (replacing std::find)
template<typename T>
typename std::vector<T>::iterator find_in_vector(
    typename std::vector<T>::iterator begin,
    typename std::vector<T>::iterator end,
    const T& value) {
    typename std::vector<T>::iterator it = begin;
    for (; it != end; ++it) {
        if (*it == value) {
            return it;
        }
    }
    return end;
}

int socket_listening(std::vector<configuration>& configs) {
    if (configs.empty()) {
        syslog(LOG_ERR, "No configurations provided");
        return -1;
    }
    std::vector<cserver> servers;
    const char* const ident = "iarbaiza xbasabe- webserver";
    openlog(ident, LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);
    syslog(LOG_USER | LOG_INFO, "Server started!");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    std::vector<int> listeners;
    std::vector<struct pollfd> pollfds;
    std::vector<size_t> client_to_server_map;
    // Set up listeners for each configuration
    for (size_t i = 0; i < configs.size(); ++i) {
        // C++98 way to add a new server
        servers.push_back(cserver());
        struct addrinfo* result = NULL;
        int status = getaddrinfo(NULL, configs[i].port.c_str(), &hints, &result);
        if (status != 0) {
            syslog(LOG_ERR, "getaddrinfo error: %s", gai_strerror(status));
            continue;
        }
        int listener_fd = -1;
        for (struct addrinfo* rptr = result; rptr != NULL; rptr = rptr->ai_next) {
            listener_fd = socket(rptr->ai_family, rptr->ai_socktype, rptr->ai_protocol);
            if (listener_fd == -1) continue;
            int optval = 1;
            if (setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
                syslog(LOG_ERR, "setsockopt failed: %s", strerror(errno));
                close(listener_fd);
                listener_fd = -1;
                continue;
            }
            // Set socket to non-blocking mode
            int flags = fcntl(listener_fd, F_GETFL, 0);
            fcntl(listener_fd, F_SETFL, flags | O_NONBLOCK);
            if (bind(listener_fd, rptr->ai_addr, rptr->ai_addrlen) == 0) {
                break;
            }
            close(listener_fd);
            listener_fd = -1;
        }
        freeaddrinfo(result);
        if (listener_fd == -1) {
            syslog(LOG_ERR, "Failed to bind to port %s", configs[i].port.c_str());
            continue;
        }
        if (listen(listener_fd, BACKLOG) == -1) {
            syslog(LOG_ERR, "Listen failed on port %s: %s",
                   configs[i].port.c_str(), strerror(errno));
            close(listener_fd);
            continue;
        }
        listeners.push_back(listener_fd);
        struct pollfd pfd;
        pfd.fd = listener_fd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollfds.push_back(pfd);
    }
    if (listeners.empty()) {
        syslog(LOG_ERR, "No listeners could be established");
        return -1;
    }
    // Main server loop
    while (true) {
        int poll_result = poll(&pollfds[0], pollfds.size(), -1);
        if (poll_result == -1) {
            if (errno == EINTR) continue;
            syslog(LOG_ERR, "Poll failed: %s", strerror(errno));
            break;
        }
        // Handle events
        //std::cout << pollfds.size() << std::endl << std::endl << std::endl << std::endl;
        for (size_t i = 0; i < pollfds.size(); ++i) {
            if (pollfds[i].revents == 0) continue;
            // Handle error conditions
            if (pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                std::vector<int>::iterator it = find_in_vector(
                    listeners.begin(), listeners.end(), pollfds[i].fd);
                if (it == listeners.end()) {
                    close(pollfds[i].fd);
                    pollfds[i].fd = -1;
                    continue;
                }
            }
            if (pollfds[i].revents & POLLIN) {
                // Check if this is a listener socket
                std::vector<int>::iterator it = find_in_vector(
                    listeners.begin(), listeners.end(), pollfds[i].fd);
                if (it != listeners.end()) {
                    size_t server_index = it - listeners.begin();
                    struct sockaddr_storage client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    int client_fd = accept(pollfds[i].fd, (struct sockaddr*)&client_addr, &addr_len);
                    //if (client_fd == -1) { //Ralentiza el siege
                        if (client_fd < 0 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
                            syslog(LOG_ERR, "Accept failed: %s", strerror(errno));
                        }
                        //continue;
                    //}
                    // Set client socket to non-blocking
                    int flags = fcntl(client_fd, F_GETFL, 0);
                    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
                    // Enforce maximum client limit
                    if (pollfds.size() >= MAX_CLIENTS) { //body_size
                        syslog(LOG_WARNING, "Max clients reached, dropping connection");
                        close(client_fd);
                        continue;
                    }
                    // Log client connection
                    char ip_str[INET6_ADDRSTRLEN];
                    if (client_addr.ss_family == AF_INET) {
                        struct sockaddr_in* s = (struct sockaddr_in*)&client_addr;
                        inet_ntop(AF_INET, &s->sin_addr, ip_str, sizeof(ip_str));
                    } else {
                        struct sockaddr_in6* s = (struct sockaddr_in6*)&client_addr;
                        inet_ntop(AF_INET6, &s->sin6_addr, ip_str, sizeof(ip_str));
                    }
                    syslog(LOG_INFO, "New connection from %s on port %s",
                           ip_str, configs[server_index].port.c_str());
                    struct pollfd new_pfd;
                    new_pfd.fd = client_fd;
                    new_pfd.events = POLLIN;
                    new_pfd.revents = 0;
                    pollfds.push_back(new_pfd);
                    client_to_server_map.push_back(server_index);
                } else {
                    // Handle client data
                    size_t client_index = i;
                    size_t server_index = client_to_server_map[client_index - listeners.size()];
                    try {
                        servers[server_index].dostuff(pollfds[i].fd, configs[server_index]);
                    } catch (const std::exception& e) {
                        syslog(LOG_ERR, "Error handling client: %s", e.what());
                        close(pollfds[i].fd);
                        pollfds[i].fd = -1;
                    }
                }
            }
        }
        // Clean up disconnected clients
        size_t write_index = listeners.size();
        size_t read_index = listeners.size();
        while (read_index < pollfds.size()) {
            if (pollfds[read_index].fd != -1) {
                if (write_index != read_index) {
                    pollfds[write_index] = pollfds[read_index];
                    client_to_server_map[write_index - listeners.size()] =
                        client_to_server_map[read_index - listeners.size()];
                }
                ++write_index;
            }
            ++read_index;
        }
        if (write_index < pollfds.size()) {
            client_to_server_map.erase(
                client_to_server_map.begin() + (write_index - listeners.size()),
                client_to_server_map.end()
            );
            pollfds.erase(pollfds.begin() + write_index, pollfds.end());
        }
    }
    // Cleanup
    for (size_t i = 0; i < listeners.size(); ++i) {
        close(listeners[i]);
    }
    for (size_t i = 0; i < pollfds.size(); ++i) {
        if (pollfds[i].fd != -1) {
            close(pollfds[i].fd);
        }
    }
    closelog();
    return 0;
}
