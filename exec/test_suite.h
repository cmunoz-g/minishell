/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_suite.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:59:41 by juramos           #+#    #+#             */
/*   Updated: 2024/04/05 09:45:13 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_SUITE_H
# define TEST_SUITE_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
#include "../libft/libft.h"

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define STDIN 8
# define FILE 9

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct s_cmd_table
{
	char				*cmd;
	char				*arg;
	int					in;
	int					out;
	char				*filename;
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}				t_cmd_table;

t_cmd_table	*init_cmd(char *cmd, char *arg, int in, int out);

#endif