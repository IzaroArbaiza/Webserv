#pragma once
#include <string>
#include <stdbool.h>

struct configuration {
            std::string port;
            std::string root;
            std::string page_404;
            std::string page_403;
            std::string page_418;
            std::string page_500;
            bool get_allowed;
            bool post_allowed;
            bool delete_allowed;
        } config;
        