/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:17:10 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/26 12:28:06 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_new_args_nbr(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

int	quote_in_middle(char *arg, char quote_type)
{
	int	i;

	i = 0;
	while (arg[i + 1])
	{
		if (arg[i] == quote_type)
			return (0);
		i++;
	}
	return (1);
}

char	*take_out_quotes(char **args, int i, t_minishell *data)
{
	char	*new_arg;
	int		j;
	int		w;

	j = 0;
	w = 0;
	new_arg = (char *)malloc(ft_strlen(args[i]) + 1);
	if (!new_arg)
		error(data, "Memory problems in mini_export");
	while (args[i][j])
	{
		if (args[i][j] != '\"' && args[i][j] != '\'')
			new_arg[w++] = args[i][j];
		j++;
	}
	new_arg[w] = '\0';
	return (new_arg);
}

void	check_possible_quotes(t_minishell *data)
{
	int		i_j_w[3];
	char	*tmp;
	char	**new_args;

	tmp = NULL;
	new_args = check_pq_init(i_j_w, data);
	while (i_j_w[0] < data->cmd_table->n_args)
	{
		if (!is_pos_variable(data->cmd_table->args[i_j_w[0]])
			&& !get_var_size(data->cmd_table->args[i_j_w[0]], false))
		{
			while (i_j_w[1] < data->cmd_table->n_args
				&& is_pos_variable(data->cmd_table->args[i_j_w[1]]))
				check_pq_loop(i_j_w, data, &tmp);
			check_pq_aux(data, i_j_w, new_args, &tmp);
		}
		else if (is_quoted_var(data->cmd_table->args[i_j_w[0]]))
		{
			new_args[i_j_w[2]] = ft_strdup(data->cmd_table->args[i_j_w[0]]);
			i_j_w[2]++;
		}
		i_j_w[0]++;
		i_j_w[1] = i_j_w[0] + 1;
	}
	check_pq_end(data, new_args, i_j_w);
}
