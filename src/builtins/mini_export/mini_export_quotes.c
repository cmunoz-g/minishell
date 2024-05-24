/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:17:10 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/24 11:23:46 by camunozg         ###   ########.fr       */
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

char	**check_pq_init(int *i_j_w, t_minishell *data)
{
	char	**new_args;

	i_j_w[0] = 0;
	i_j_w[1] = 1;
	i_j_w[2] = 0;
	new_args = (char **)malloc(sizeof(char *) * (data->cmd_table->n_args + 1));
	if (!new_args)
		error(data, "Memory problems in mini_export");
	return (new_args);
}

void	check_pq_loop_aux(int *i_j_w, t_minishell *data, char **tmp)
{
	char	*middle;

	if (!quote_in_middle(data->cmd_table->args[i_j_w[1]],
			(*(data->cmd_table->args[i_j_w[1]] - 1))))
	{
		middle = take_out_quotes(data->cmd_table->args, i_j_w[1], data);
		ft_strlcat(*tmp, middle, (ft_strlen(*tmp)
				+ ft_strlen(data->cmd_table->args[i_j_w[1]]) + 1));
		free(middle);
	}
	else
		ft_strlcat(*tmp, data->cmd_table->args[i_j_w[1]],
			(ft_strlen(*tmp) + ft_strlen(data->cmd_table->args[i_j_w[1]])));
	data->cmd_table->args[i_j_w[1]]--;
}

void	check_pq_loop(int *i_j_w, t_minishell *data, char **tmp)
{
	if (data->cmd_table->args[i_j_w[1]]
		&& !is_quoted_var(data->cmd_table->args[i_j_w[1]]))
	{
		if (!(*tmp))
		{
			*tmp = (char *)malloc(ft_strlen(data->cmd_table->args[i_j_w[0]])
					+ ft_strlen(data->cmd_table->args[i_j_w[1]]) + 1);
			if (!(*tmp))
				error(data, "Memory problems in mini_export");
			ft_strlcpy(*tmp, data->cmd_table->args[i_j_w[0]],
				ft_strlen(data->cmd_table->args[i_j_w[0]]) + 1);
		}
		data->cmd_table->args[i_j_w[1]]++;
		check_pq_loop_aux(i_j_w, data, tmp);
	}
	i_j_w[1]++;
}

void	check_pq_end(t_minishell *data, char **new_args, int *i_j_w)
{
	new_args[i_j_w[2]] = NULL;
	free_arr(data->cmd_table->args);
	data->cmd_table->args = new_args;
	data->cmd_table->n_args = get_new_args_nbr(data->cmd_table->args);
}

void	check_pq_aux(t_minishell *data, int *i_j_w, char **new_args, char **tmp)
{
	if (i_j_w[1] == i_j_w[0] + 1)
		new_args[i_j_w[2]] = ft_strdup(data->cmd_table->args[i_j_w[0]]);
	else
		new_args[i_j_w[2]] = ft_strdup(*tmp);
	if (!new_args[i_j_w[2]])
		error(data, "Memory problems in mini_export");
	i_j_w[2]++;
	free(*tmp);
	*tmp = NULL;
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
