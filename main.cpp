#include "cserver.hpp"
#define SERVER_PORT                "8080" //viene de enviroment
#define ERROR_IN_INPUT             9
#define BACKLOG                   10
#define NUM_FDS                    5

void error (char *msg);

int main (int argc, char **argv)
{
    //signal(SIGINT, (void (*)(int))(&sigintHandler));
    cserver server_action;
    const char * const ident = "xbasabe- trainning server"; //viene user en el enviroment
    openlog (ident, LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);
    syslog (LOG_USER | LOG_INFO, "%s", "server started!");
    struct addrinfo hints;
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_PASSIVE;    /* for wildcard IP address */
    struct addrinfo *result;
    int s;
    if ((s = getaddrinfo (NULL, SERVER_PORT, &hints, &result)) != 0) {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        exit (EXIT_FAILURE);
    }
    /* Scan through the list of address structures returned by
       getaddrinfo. Stop when the the socket and bind calls are successful. */
    int listener, optval = 1;
    socklen_t length;
    struct addrinfo *rptr;
    for (rptr = result; rptr != NULL; rptr = rptr -> ai_next) {
        listener = socket (rptr -> ai_family, rptr -> ai_socktype,
                       rptr -> ai_protocol); // listener == fd server socket
        if (listener == -1)
            continue;
        if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (int)) == -1)
            error("setsockopt");
        if (bind (listener, rptr -> ai_addr, rptr -> ai_addrlen) == 0)  // Success
            break;
        if (close (listener) == -1)
            error ("close");
    }
    if (rptr == NULL) {               // Not successful with any address
        fprintf(stderr, "Not able to bind\n");
        exit (EXIT_FAILURE);
    }
    freeaddrinfo (result);
    // Mark socket for accepting incoming connections using accept, el socket que escucha peticiones connect del cliente
    if (listen (listener, BACKLOG) == -1)
        error ("listen");
    nfds_t nfds = 0;
    int maxfds = 0, numfds = 0;
    struct pollfd pollfds[NUM_FDS];
    memset(&pollfds, 0, sizeof(struct pollfd));
    //struct pollfd *pollfds;
    //pollfds = (pollfd*)malloc(NUM_FDS * sizeof (struct pollfd));
    //if (pollfds == NULL)
	//    error ("malloc");
    maxfds = NUM_FDS;
    pollfds[0].fd = listener;
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;
    numfds = 1;
    socklen_t addrlen;
    struct sockaddr_storage client_saddr;
    char str [INET6_ADDRSTRLEN];
    struct sockaddr_in  *ptr;
    struct sockaddr_in6  *ptr1;
    struct tnode *root = NULL;
    while (1) {
        // monitor readfds for readiness for reading
	    nfds = numfds;
	    if (poll (pollfds, nfds, -1) == -1)
	        error ("poll");
        // Some sockets are ready. Examine readfds
        for (int i = 0; i < (nfds + 1); i++) {
            //inicializar pollfds, ¿memset? ejemplo memset(&hints, 0, sizeof (struct addrinfo));
            //memset(&pollfds, 0, sizeof(struct pollfd));
		    if (pollfds[i].fd <= 0) // file desc == 0 is not expected, as these are socket fds and not stdin
                continue;
            if ((pollfds[i].revents & POLLIN) == POLLIN) { // fd is ready for reading
                if (pollfds[i].fd == listener) {  // request for new connection
                    addrlen = sizeof (struct sockaddr_storage);
                    int fd_new; //new client or new connection?
                    if ((fd_new = accept (listener, (struct sockaddr *) &client_saddr, &addrlen)) == -1){
                        error ("accept");
                        maxfds += NUM_FDS;
		            }
                    numfds++;
                    pollfds[numfds - 1].fd = fd_new;
                    pollfds[numfds - 1].events = POLLIN;
                    pollfds[numfds - 1].revents = 0;
                    // print IP address of the new client ¿se admiten ambos protocolos? AFINET y AF_INET6?
                    if (client_saddr.ss_family == AF_INET) {
                        ptr = (struct sockaddr_in *) &client_saddr;
                        inet_ntop (AF_INET, &(ptr -> sin_addr), str, sizeof (str));
                    }
                    else if (client_saddr.ss_family == AF_INET6) {
                        ptr1 = (struct sockaddr_in6 *) &client_saddr;
	                    inet_ntop (AF_INET6, &(ptr1 -> sin6_addr), str, sizeof (str));
                    }
                    else
                    {
                        ptr = NULL;
                        fprintf (stderr, "Address family is neither AF_INET nor AF_INET6\n");
                    }
                    if (ptr)
                        syslog (LOG_USER | LOG_INFO, "%s %s", "Connection from client", str); // quitar syslog ¿equivalente, es necesario?
                }
                else  // data from an existing connection, receive it && do stuf 
                {
                    //dostuff(pollfds[i].fd); //CLASS SERVER, DO STUFF
                    //cserver server_action;
                    server_action.dostuff(pollfds[i].fd);
                    numfds--; //quitamos fd de la lista de fd activos // revisar porque no reutilizamos el fd
                }
            } // if (fd == ... POLLIN
        } // for
    } // while (1) solo sale con error o Ctrl+C
    close(listener);
    for(int i = 0; i < (nfds + 1); i++)
        close(pollfds[i].fd);
    exit (EXIT_SUCCESS);
} // main

void error (/*const*/ char *msg) //actualiza error y sale del bucle, cierra el server en c++ pasar a exception
{
    perror (msg);
    exit (1);
}

void sigintHandler(int signal, request * entry) { //parametros listener pollfds nfds o crear clase server y pasar el server
    if (signal == SIGINT) {
        std::cout << "Caught SIGINT (Ctrl+C)!" << std::endl;
        // Perform any necessary cleanup or actions
        //if (entry)
        //    delete entry;
        //limpiar alojado para poll?
        //close(listener);
        //for(int i = 0; i < (nfds + 1); i++)
        //    close(pollfds[i].fd);
    }
}
