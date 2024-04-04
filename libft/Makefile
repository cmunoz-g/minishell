# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 16:43:25 by juramos           #+#    #+#              #
#    Updated: 2024/04/03 13:34:55 by juramos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
B_NAME = libftbonus.a
CC = gcc
INCLUDE = ../include
CFLAGS = -Wall -Werror -Wextra -I
SRC = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
	ft_isprint.c ft_toupper.c ft_tolower.c ft_strlen.c \
	ft_atoi.c ft_strlcpy.c ft_strlcat.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_strnstr.c ft_strdup.c ft_memset.c \
	ft_bzero.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c \
	ft_calloc.c ft_substr.c ft_strjoin.c ft_strtrim.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
	ft_putnbr_fd.c ft_strmapi.c ft_striteri.c \
	ft_split.c ft_itoa.c ft_putnbr_base_fd.c \
	get_next_line.c get_next_line_utils.c
BONUS_SRC = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
	ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c
BONUS_OBJ = $(BONUS_SRC:.c=.o)
OBJ = $(SRC:.c=.o)
MAKEFLAGS += --no-print-directory
.SILENT:

# Colors
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all: $(NAME)

bonus: $(B_NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $?
	@echo "$(YELLOW)libft compiled!$(DEF_COLOR)"

$(B_NAME): $(BONUS_OBJ)
	@ar rcs $(B_NAME) $?
	@ar rcs $(NAME) $?
	@echo "$(YELLOW)libft bonus compiled!$(DEF_COLOR)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	@rm -f $(NAME) $(B_NAME)

re: fclean all

so:
	@$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRC)
	@gcc -nostartfiles -shared -o libft.so $(OBJ)

.PHONY: all clean fclean re bonus so
