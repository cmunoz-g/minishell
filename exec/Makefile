# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 16:43:25 by juramos           #+#    #+#              #
#    Updated: 2024/04/05 09:37:41 by juramos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME 		= 	miniparse

# Compiler
CC 			= 	gcc
CFLAGS 		= 	-Wall -Werror -Wextra

# Sources
SRC_FILES 	= 	test_suite
SRC 		=	$(addsuffix .c, $(SRC_FILES))

# Objects
OBJ 		=	$(addsuffix .o, $(SRC_FILES))

# libft
LIBFT_PATH	= 	../libft/
LIBFT_NAME	= 	libft.a
LIBFT		= 	$(LIBFT_PATH)$(LIBFT_NAME)

# Includes
INC			=	-I ./../libft

# Colors
DEF_COLOR 	= 	\033[0;39m
GRAY 		= 	\033[0;90m
RED 		= 	\033[0;91m
GREEN 		=	\033[0;92m
YELLOW 		=	\033[0;93m
BLUE 		=	\033[0;94m
MAGENTA 	=	\033[0;95m
CYAN 		= 	\033[0;96m
WHITE 		= 	\033[0;97m

# Config
OBJF 		=	.cache_exists
MAKEFLAGS 	+=	--no-print-directory
.SILENT:

###

all: $(LIBFT) $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(INC)
	@echo "$(GREEN)minishell compiled!$(DEF_COLOR)"

libft:
	@make -sC $(LIBFT_PATH)

$(LIBFT):
	@echo "Making libft..."
	@make -sC $(LIBFT_PATH)

clean:
	@rm -f *.o
	@make clean -sC $(LIBFT_PATH)
	@echo "$(BLUE)miniparse object files cleaned!$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@make fclean -sC $(LIBFT_PATH)
	@echo "$(CYAN)miniparse executable files cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for miniparse!$(DEF_COLOR)"

norm:
	@norminette $(SRC) | grep -v Norme -B1 || true

.PHONY: all clean fclean re norm libft