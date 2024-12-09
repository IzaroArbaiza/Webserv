
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include "parse.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <sys/ioctl.h>
#include <csignal>
#include "request.hpp"
#include "getHandler.hpp"
#include "postHandler.hpp"
#include "deleteHandler.hpp"
#include "cgiHandler.hpp"
#include "response.hpp"
#include "getRequest.hpp"
#include "postRequest.hpp"
#include "RequestFactory.hpp"
#include "cserver.hpp"

#include <cstring>

#define CRLF "\r\n"