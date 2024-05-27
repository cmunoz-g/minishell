# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 16:43:25 by juramos           #+#    #+#              #
#    Updated: 2024/05/27 08:57:17 by camunozg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME 		= 	minishell

# Compiler
CC 			= 	gcc
CFLAGS 		= 	-Wall -Werror -Wextra -g3 -fsanitize=address

# Sources
# Removed signal_handler\ for now
SRC_DIR 	= 	src/
SRC_FILES 	= 	main\
				minishell_loop\
				init\
				history\
				variables/local_variables\
				variables/local_variables_utils\
				variables/local_variables_checks\
				variables/local_variables_utils_aux\
				variables/local_variables_quotes\
				variables/sh_lvl\
				lexer/lexer_utils\
				lexer/lexer\
				lexer/tokens\
				lexer/lexer_checks\
				lexer/lexer_token_checks\
				syntax/syntax\
				syntax/comments_end\
				parser/cmd_table_utils\
				parser/cmd_table\
				parser/parser_utils\
				parser/parser\
				utils/clean\
				utils/clean_cmd\
				utils/extra_libft\
				utils/error\
				exec/arr_utils\
				exec/exec\
				exec/error_handlers\
				exec/redirections\
				exec/exec_utils\
				exec/heredoc\
				exec/expand\
				exec/expand_utils\
				exec/main_exec\
				builtins/mini_cd\
				builtins/mini_exit\
				builtins/mini_pwd\
				builtins/mini_echo\
				builtins/mini_env\
				builtins/mini_unset/mini_unset\
				builtins/mini_unset/mini_unset_utils\
				builtins/mini_export/mini_export\
				builtins/mini_export/mini_export_order\
				builtins/mini_export/mini_export_utils\
				builtins/mini_export/mini_export_utils_aux\
				builtins/mini_export/mini_export_operations\
				builtins/mini_export/mini_export_quotes\
				builtins/mini_export/mini_export_quotes_aux\
				builtins/mini_export/mini_export_dec\
				builtins/utils_builtins\
				signals\
				
SRC 		=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))

# Objects
OBJ_DIR 	= 	obj/
OBJ_DIRS	=	$(sort $(dir $(OBJ)))
OBJ 		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

# libft
LIBFT_PATH	= 	libft/
LIBFT_NAME	= 	libft.a
LIBFT		= 	$(LIBFT_PATH)$(LIBFT_NAME)

# build path
B_PATH = build/

# Includes
INC_MS		= -I include/ \
			-I /usr/local/Cellar/readline/8.2.10/include

# Readline Compile
RL_CMP		= -L/usr/local/Cellar/readline/8.2.10/lib

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

all: $(B_PATH) $(LIBFT) $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
	@$(CC) $(CFLAGS) $(INC_MS) $(INC) -c $< -o $@

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p  $(OBJ_DIRS)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(INC) -lreadline $(RL_CMP)
	@echo "$(GREEN)minishell compiled!$(DEF_COLOR)"

$(B_PATH):
	@mkdir $(B_PATH)

libft:
	@make -sC $(LIBFT_PATH)

$(LIBFT):
	@echo "Making libft..."
	@make -sC $(LIBFT_PATH)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(B_PATH)
	@make clean -sC $(LIBFT_PATH)
	@echo "$(BLUE)minishell object files cleaned!$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@make fclean -sC $(LIBFT_PATH)
	@echo "$(CYAN)minishell executable files cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for minishell!$(DEF_COLOR)"

norm:
	@norminette $(SRC) | grep -v Norme -B1 || true

miniexec:
	make -sC src/exec

.PHONY: all clean fclean re norm libft