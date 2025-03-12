# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/30 11:57:17 by xbasabe-          #+#    #+#              #
#    Updated: 2025/03/12 17:12:50 by iarbaiza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
SRC :=  src/cgiHandler.cpp \
		src/cserver.cpp \
		src/deleteHandler.cpp \
		src/deleteRequest.cpp \
		src/errorHandler.cpp \
		src/getHandler.cpp \
		src/getRequest.cpp\
		src/postHandler.cpp \
		src/postRequest.cpp \
		src/request.cpp\
		src/RequestFactory.cpp \
		src/response.cpp \
		src/uploadHandler.cpp \
		src/Parse.cpp \
		src/ConfFile.cpp \
		src/Location.cpp \
		src/ServerConf.cpp \
		src/main.cpp \

CXX = c++
RM := rm -rf
CPPFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c++98 -g3
OBJS = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

$(OBJS):%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean :
		$(RM) $(OBJS)

fclean : clean
		$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re