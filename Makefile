# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/30 11:57:17 by xbasabe-          #+#    #+#              #
#    Updated: 2024/11/27 11:22:02 by xbasabe-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := server
SRC :=	cserver.cpp \
		response.cpp \
		postHandler.cpp \
		request.cpp\
		parse.cpp\
		getRequest.cpp\
		getHandler.cpp \
		errorHandler.cpp \
		deleteHandler.cpp \
		cgiHandler.cpp \
		postRequest.cpp \
		RequestFactory.cpp \
		uploadHandler.cpp \
		deleteRequest.cpp \
		main.cpp \

CXX = c++
RM := rm -rf
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 
OBJS = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

$(OBJS):%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
		$(RM) $(OBJS)

fclean : clean
		$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re