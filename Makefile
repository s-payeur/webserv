# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/26 23:29:42 by spayeur           #+#    #+#              #
#    Updated: 2023/01/09 15:19:50 by spayeur          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 Informations                                 #
# **************************************************************************** #

NAME		=	webserv

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98
INCS_DIR	=	incs
SRCS_DIR	=	srcs

# **************************************************************************** #
#                                    C0l0r$                                    #
# **************************************************************************** #

# Regular Colors
BLACK		:= $(shell tput -Txterm setaf 0)
RED			:= $(shell tput -Txterm setaf 1)
GREEN		:= $(shell tput -Txterm setaf 2)
YELLOW		:= $(shell tput -Txterm setaf 3)
BLUE		:= $(shell tput -Txterm setaf 4)
MAGENTA		:= $(shell tput -Txterm setaf 5)
CYAN		:= $(shell tput -Txterm setaf 6)
WHITE		:= $(shell tput -Txterm setaf 7)

# Bold
BOLD		:= $(shell tput -Txterm bold)
BBLACK		= $(BOLD)$(BLACK)
BRED		= $(BOLD)$(RED)
BGREEN		= $(BOLD)$(GREEN)
BYELLOW		= $(BOLD)$(YELLOW)
BBLUE		= $(BOLD)$(BLUE)
BMAGENTA	= $(BOLD)$(MAGENTA)
BCYAN		= $(BOLD)$(CYAN)
BWHITE		= $(BOLD)$(WHITE)
END			:= $(shell tput -Txterm sgr0)

# Clear
CLEAR		:= $(shell tput -Txterm clear)
CLEAR_LINE	:= $(shell tput -Txterm el)

# **************************************************************************** #
#                                 Source files                                 #
# **************************************************************************** #

# ******************************** Mandatory ********************************* #

SRCS_DEFAULT_FILES	=	main

SRCS_DEFAULT		=	$(addprefix $(SRCS_DIR)/, $(SRCS_DEFAULT_FILES))

SRCS_CLASSES_FILES	=	Http \
						Server \
						Location

SRCS_CLASSES		=	$(addprefix $(SRCS_DIR)/classes/, $(SRCS_CLASSES_FILES))

SRCS_PARSING_FILES	=	a

SRCS_PARSING		=	$(addprefix $(SRCS_DIR)/parsing/, $(SRCS_PARSING_FILES))

SRCS_PROJECT		=	$(addsuffix .cpp, $(SRCS_DEFAULT) $(SRCS_CLASSES) $(SRCS_PARSING))

# **************************************************************************** #
#                                 Object files                                 #
# **************************************************************************** #

OBJS_PROJECT		=	$(SRCS_PROJECT:.cpp=.o)

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

%.o:		%.cpp
			@printf "$(CLEAR_LINE)$(BMAGENTA)$@\r$(END)"
			@$(CC) $(CFLAGS) I $(INCS_DIR) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS_PROJECT)
			@$(CC) $(CFLAGS) $(OBJS_PROJECT) -o $(NAME)
			@echo "$(CLEAR_LINE)$(CYAN)$(NAME) $(BGREEN)has been build !$(END)"

clean:
			@echo "$(BBLUE)Cleaning...$(END)"
			@echo "$(CYAN)$(words $(sort $(wildcard $(OBJS_PROJECT)))) object file(s) $(BGREEN)has/have been removed.$(END)"
			@rm -f $(OBJS_PROJECT)

fclean:		clean
ifneq (,$(wildcard $(NAME)))
			@rm -f $(NAME)
			@echo "$(CYAN)$(NAME)$(END) $(BGREEN)has been removed.$(END)"
endif

re:			fclean all

.PHONY:		all clean fclean re
