# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 16:43:25 by juramos           #+#    #+#              #
#    Updated: 2024/04/04 17:44:59 by juramos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME 		= 	minishell
EXEC 		= 	exec_mini
PARSE		=	parse_mini

# Compiler
CC 			= 	gcc
CFLAGS 		= 	-Wall -Werror -Wextra

# Sources
SRC_DIR 	= 	src/
SRC_FILES 	= 	main utils
SRC 		=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))

# Objects
OBJ_DIR 	= 	obj/
OBJ 		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

# libft
LIBFT_PATH	= 	libft/
LIBFT_NAME	= 	libft.a
LIBFT		= 	$(LIBFT_PATH)$(LIBFT_NAME)

# Includes
INC			=	-I ./libft

######### testing only
# Exec Sources
EXEC_SRC_DIR 	= 	exec/
EXEC_FILES 		= 	main utils
EXEC_SRC		=	$(addprefix $(EXEC_SRC_DIR), $(addsuffix .c, $(EXEC_FILES)))

# Exec Objects
EXEC_OBJ_DIR 	= 	exec_obj/
EXEC_OBJ 		=	$(addprefix $(EXEC_OBJ_DIR), $(addsuffix .o, $(EXEC_FILES)))

# Parsing Sources
PARSE_SRC_DIR 	= 	parsing/
PARSE_FILES 	= 	main utils
PARSE_SRC		=	$(addprefix $(PARSE_SRC_DIR), $(addsuffix .c, $(PARSE_FILES)))

# Parse Objects
PARSE_OBJ_DIR 	= 	parse_obj/
PARSE_OBJ 		=	$(addprefix $(PARSE_OBJ_DIR), $(addsuffix .o, $(PARSE_FILES)))
######### testing only

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

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJF):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(INC)
	@echo "$(GREEN)minishell compiled!$(DEF_COLOR)"

######### testing only

exec: $(LIBFT) $(EXEC)

$(EXEC_OBJ_DIR)%.o: $(EXEC_SRC_DIR)%.c | $(EXEC_OBJF)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(EXEC_OBJF):
	@mkdir -p $(EXEC_OBJ_DIR)

$(EXEC): $(EXEC_OBJ)
	@$(CC) $(CFLAGS) -o $(EXEC) $(EXEC_OBJ) $(LIBFT) $(INC)
	@echo "$(GREEN)exec compiled!$(DEF_COLOR)"

parse: $(LIBFT) $(PARSE)

$(PARSE_OBJ_DIR)%.o: $(PARSE_SRC_DIR)%.c
	@mkdir -p $(PARSE_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(PARSE): $(PARSE_OBJ)
	@$(CC) $(CFLAGS) -o $(PARSE) $(PARSE_OBJ) $(LIBFT) $(INC)
	@echo "$(GREEN)parse_mini compiled!$(DEF_COLOR)"
######### testing only

libft:
	@make -sC $(LIBFT_PATH)

$(LIBFT):
	@echo "Making libft..."
	@make -sC $(LIBFT_PATH)

clean:
	@rm -rf $(OBJ_DIR) $(PARSE_OBJ_DIR) $(EXEC_OBJ_DIR)
	@make clean -sC $(LIBFT_PATH)
	@echo "$(BLUE)minishell object files cleaned!$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME) $(PARSE) $(EXEC)
	@make fclean -sC $(LIBFT_PATH)
	@echo "$(CYAN)minishell executable files cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for minishell!$(DEF_COLOR)"

norm:
	@norminette $(SRC) | grep -v Norme -B1 || true

.PHONY: all clean fclean re norm libft parse exec