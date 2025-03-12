/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:28:00 by iarbaiza          #+#    #+#             */
/*   Updated: 2025/03/12 18:28:01 by iarbaiza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MAX_EVENTS 5000
#include "../include/cserver.hpp"

volatile sig_atomic_t g_running = 1;

void signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        std::cout << std::endl << "Signal SIGINT (Ctrl+C), closing server..." << std::endl;
        g_running = 0;
    }
}

void error(std::string msg);
int parse_config(int argc, char const * argv[], std::vector<configuration> &confis);
void fill_config(int n, std::vector<configuration> &confis);
int epoll_listener(std::vector<configuration>& configs);


int main(int argc, char const *argv[])
{
    signal(SIGINT, signal_handler);
    int server_amount = 0;
    std::vector<configuration > confis;
    server_amount = parse_config(argc, argv, confis);
    if (server_amount > 0) {
        fill_config(server_amount , confis);
        epoll_listener(confis);
    }
    return (0);
}

void error(std::string msg) {
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
			return (-1);
		}
	} else {
		std::cout << "Invalid amount of arguments. Try again" << std::endl;
        return (-1);
	}
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
    int errors[] = {200, 204, 308, 403, 404, 405, 406, 409, 413, 418, 500, 505};

    for (size_t i = 0; i < sizeof(errors)/sizeof(errors[0]); ++i) {
        int codigoError = errors[i];

        if (mapaError.find(codigoError) == mapaError.end()) {
            mapaError[codigoError] = "error_pages/" + to_string(codigoError) + ".html";
        }
    }
}

void fill_config(int n, std::vector<configuration> &confis) {
    for (int i = 0; i < n; i++) {
        std::string rut = "/home/";
        confis.at(i).user = getenv("USER");
        rut.append(confis.at(i).user);
        char* cwd = getcwd(NULL, 0);
        if (cwd == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
        rut.append(cwd);
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
        confis.at(i).init_root = confis.at(i).root;
        if (confis.at(i).activeDirectory.empty())
            confis.at(i).activeDirectory = rut;
        if (confis.at(i).index.empty())
            confis.at(i).index = "./index";
        if (confis.at(i).body_size <= 0)
            confis.at(i).body_size = 1024;
        agregarErroresPredeterminados(confis.at(i).errors);
        for (int j = 0; j < (int)confis.at(i).errors.size(); j++) {
            htmlButtom(confis.at(i), confis.at(i).errors[j]);
        }
    }
}

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

int epoll_listener(std::vector<configuration>& configs) {
    int event_count, i;
    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    
    if (epoll_fd == -1) {
        fprintf(stderr, "Failed to create epoll file descriptor\n");
        return 1;
    }
    if (configs.empty()) {
        syslog(LOG_ERR, "No configurations provided");
        return -1;
    }
    std::vector<int> server_sockets;
    const char* const ident = "iarbaiza xbasabe- webserver";
    openlog(ident, LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);
    syslog(LOG_USER | LOG_INFO, "Server started!");

    for (std::vector<configuration>::iterator config = configs.begin(); config != configs.end(); ++config) {
        struct sockaddr_in server_addr;
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (server_fd < 0) {
            syslog(LOG_ERR, "Failed to create socket");
            continue;
        }
        int flags = fcntl(server_fd, F_GETFL, 0);
        if (flags < 0) {
            syslog(LOG_ERR, "Failed to get socket flags");
            close(server_fd);
            continue;
        }
        if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
            syslog(LOG_ERR, "Failed to set non-blocking mode");
            close(server_fd);
            continue;
        }
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            syslog(LOG_ERR, "Failed to set socket options");
            close(server_fd);
            continue;
        }
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(config->host.c_str());
        server_addr.sin_port = htons(atoi(config->port.c_str()));

        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            syslog(LOG_ERR, "Failed to bind to port %d", atoi(config->port.c_str()));
            close(server_fd);
            continue;
        }
        if (listen(server_fd, SOMAXCONN) < 0) {
            syslog(LOG_ERR, "Failed to listen on socket");
            close(server_fd);
            continue;
        }

        memset(&event, 0, sizeof(event));
        event.events = EPOLLIN;
        event.data.fd = server_fd;
        
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event)) {
            syslog(LOG_ERR, "Failed to add server socket to epoll");
            close(server_fd);
            continue;
        }
        server_sockets.push_back(server_fd);
        syslog(LOG_INFO, "Listening on port %d", atoi(config->port.c_str()));
    }

    if (server_sockets.empty()) {
        syslog(LOG_ERR, "No server sockets created");
        close(epoll_fd);
        return -1;
    }

    std::vector<cserver> servers;
    while (g_running) {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
        
        if (event_count < 0) {
            if (errno == EINTR) continue;
            syslog(LOG_ERR, "Epoll wait failed");
            break;
        }

        std::cout << "Server listening..." << std::endl;
        
        for (i = 0; i < event_count; i++) {
            bool is_server_socket = false;
            size_t config_index = 0;
            
            for (size_t j = 0; j < server_sockets.size(); ++j) {
                if (server_sockets[j] == events[i].data.fd) {
                    is_server_socket = true;
                    config_index = j;
                    break;
                }
            }
            if (is_server_socket) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(events[i].data.fd, (struct sockaddr*)&client_addr, &client_len);
                
                if (client_fd < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        syslog(LOG_ERR, "Accept failed");
                    }
                    continue;
                }
                int flags = fcntl(client_fd, F_GETFL, 0);
                if (flags >= 0) {
                    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
                }
                memset(&event, 0, sizeof(event));
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event)) {
                    syslog(LOG_ERR, "Failed to add client socket to epoll");
                    close(client_fd);
                    continue;
                }
                servers.push_back(cserver());
            } else {
                if (events[i].events & EPOLLIN) {
                    if (config_index < configs.size()) {
                        servers.back().dostuff(events[i].data.fd, configs[config_index]);
                    }
                }
                if (events[i].events & (EPOLLHUP | EPOLLERR)) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
            }
        }
    }
    for (std::vector<int>::iterator it = server_sockets.begin(); 
         it != server_sockets.end(); ++it) {
        close(*it);
    }
    if (close(epoll_fd)) {
        std::cout << stderr << ", Failed to close epoll file descriptor" << std::endl; 
        return 1;
    }
    return 0;
}

