/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:55:47 by camunozg          #+#    #+#             */
/*   Updated: 2024/05/24 10:01:42 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_quote_var_space(t_quotes *quote, t_cmd_table *tmp, int *i_j)
{
	quote->qt = true;
	quote->qtt = tmp->args[i_j[0]][i_j[1]];
}

int	get_new_var_space(t_cmd_table *tmp)
{
	int			res;
	int			i_j[2];
	t_quotes	quote;

	init_get_new_var_space(&res, i_j, &quote);
	while (tmp->args[i_j[0]])
	{
		while (tmp->args[i_j[0]][i_j[1]])
		{
			if (tmp->args[i_j[0]][i_j[1]] == quote.qtt
				&& quote.qt == true)
				quote.qt = false;
			else if (tmp->args[i_j[0]][i_j[1]] == '\''
				|| tmp->args[i_j[0]][i_j[1]] == '\"')
				set_quote_var_space(&quote, tmp, i_j);
			if (quote.qt == true || (tmp->args[i_j[0]][i_j[1]] != '\''
				&& tmp->args[i_j[0]][i_j[1]] != '\"'))
				res++;
			i_j[1]++;
		}
		i_j[0]++;
		i_j[1] = 0;
	}
	return (res);
}

void	init_var_quotes(int *i_j_w, t_cmd_table **tmp, int *ns, t_quotes *qt)
{
	qt->qtt = '\0';
	qt->qt = false;
	i_j_w[0] = 0;
	i_j_w[1] = 0;
	i_j_w[2] = ft_strlen((*tmp)->cmd);
	*ns = get_new_var_space((*tmp));
}

void	var_quotes_loop(t_cmd_table **tmp, int *i_j_w, t_quotes *qt, char *n)
{
	while ((*tmp)->args[i_j_w[0]][i_j_w[1]])
	{
		if ((*tmp)->args[i_j_w[0]][i_j_w[1]] == qt->qtt && qt->qt == true)
			qt->qt = false;
		if (qt->qt == false && ((*tmp)->args[i_j_w[0]][i_j_w[1]] == '\''
			|| (*tmp)->args[i_j_w[0]][i_j_w[1]] == '\"'))
		{
			qt->qt = true;
			qt->qtt = (*tmp)->args[i_j_w[0]][i_j_w[1]];
		}
		if ((qt->qt == true && qt->qtt != (*tmp)->args[i_j_w[0]][i_j_w[1]])
			|| ((*tmp)->args[i_j_w[0]][i_j_w[1]] != '\''
			&& (*tmp)->args[i_j_w[0]][i_j_w[1]] != '\"'))
		{
			n[i_j_w[2]] = (*tmp)->args[i_j_w[0]][i_j_w[1]];
			i_j_w[2]++;
		}
		i_j_w[1]++;
	}
}

void	variable_with_quotes(t_cmd_table **tmp, t_minishell *data)
{
	int			i_j_w[3];
	int			new_space;
	char		*new_cmd;
	t_quotes	quote;

	init_var_quotes(i_j_w, tmp, &new_space, &quote);
	new_cmd = (char *)malloc(i_j_w[2] + new_space + 1);
	if (!new_cmd)
		error(data, "Could not parse local variable");
	ft_strlcpy(new_cmd, (*tmp)->cmd, (size_t)(i_j_w[2] + 1));
	while (i_j_w[0] < (*tmp)->n_args)
	{
		var_quotes_loop(tmp, i_j_w, &quote, new_cmd);
		i_j_w[1] = 0;
		i_j_w[0]++;
	}
	new_cmd[i_j_w[2]] = '\0';
	free_arr((*tmp)->args);
	(*tmp)->args = NULL;
	free((*tmp)->cmd);
	(*tmp)->cmd = new_cmd;
	(*tmp)->n_args = 0;
}
