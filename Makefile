# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iarbaiza <iarbaiza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/30 11:57:17 by xbasabe-          #+#    #+#              #
#    Updated: 2025/02/07 11:27:05 by iarbaiza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := server
SRC :=	cgiHandler.cpp \
		cserver.cpp \
		deleteHandler.cpp \
		deleteRequest.cpp \
		errorHandler.cpp \
		getHandler.cpp \
		getRequest.cpp\
		postHandler.cpp \
		postRequest.cpp \
		request.cpp\
		RequestFactory.cpp \
		response.cpp \
		uploadHandler.cpp \
		main.cpp \
		parseo/Parse.cpp \
		parseo/ConfFile.cpp \
		parseo/Location.cpp \
		parseo/ServerConf.cpp \

CXX = c++
RM := rm -rf
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++98 -g3 #-fsanitize=address,leak
# -Werror
OBJS = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS):%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
		$(RM) $(OBJS)

fclean : clean
		$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re