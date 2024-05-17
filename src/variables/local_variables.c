/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:16:35 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/17 11:09:15 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

void	change_var_value(char *cmd, t_variable **loc, int l, t_minishell *data)
{
	t_variable	*it;
	int			i;
	int			value_size;
	int			name_size;

	value_size = get_var_size(cmd, false);
	name_size = get_var_size(cmd, true);
	it = *loc;
	i = 0;
	while (i < l && it)
	{
		it = it->next;
		i++;
	}
	free(it->value);
	it->value = (char *)malloc(sizeof(char) * (value_size + 1));
	if (!it->value)
		error(data, "Memory problems while creating a new variable");
	ft_strlcpy(it->value, cmd + name_size + 1, value_size + 1);
}

void	local_variables_aux(t_minishell *data, t_cmd_table *tmp)
{
	int	laps;

	laps = check_new_var(tmp->cmd, data->local_vars);
	if (laps < 0)
		create_new_variable(tmp->cmd, &(data->local_vars), data);
	else
		change_var_value(tmp->cmd, &(data->local_vars), laps, data);
}

void	init_get_new_var_space(int *res, int *i_j, t_quotes *quote)
{
	i_j[0] = 0;
	i_j[1] = 0;
	*res = 0;
	quote->qtt = '\0';
	quote->qt = false;
}

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

void	init_var_quotes(int *i_j_w, t_cmd_table **tmp, int *new_space, t_quotes *quote)
{
	quote->qtt = '\0';
	quote->qt = false;
	i_j_w[0] = 0;
	i_j_w[1] = 0;
	i_j_w[2] = ft_strlen((*tmp)->cmd);
	*new_space = get_new_var_space((*tmp)); 
}

void	var_quotes_loop(t_cmd_table **tmp, int *i_j_w, t_quotes *quote, char *new_cmd)
{
	while ((*tmp)->args[i_j_w[0]][i_j_w[1]])
	{
		if ((*tmp)->args[i_j_w[0]][i_j_w[1]] == quote->qtt && quote->qt == true)
			quote->qt = false;
		if (quote->qt == false && ((*tmp)->args[i_j_w[0]][i_j_w[1]] == '\''
			|| (*tmp)->args[i_j_w[0]][i_j_w[1]] == '\"'))
		{
			quote->qt = true;
			quote->qtt = (*tmp)->args[i_j_w[0]][i_j_w[1]];
		}
		if ((quote->qt == true && quote->qtt != (*tmp)->args[i_j_w[0]][i_j_w[1]])
			|| ((*tmp)->args[i_j_w[0]][i_j_w[1]] != '\''
			&& (*tmp)->args[i_j_w[0]][i_j_w[1]] != '\"'))
		{
			new_cmd[i_j_w[2]] = (*tmp)->args[i_j_w[0]][i_j_w[1]];
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

void	clean_table_var(t_cmd_table **tmp, t_cmd_table **next, t_minishell *data)
{
	int			i;

	i = 0;
	free((*tmp)->cmd);
	free_arr((*tmp)->args);
	while (i < (*tmp)->n_redirections)
	{
		free((*tmp)->redirections[i]->value);
		free((*tmp)->redirections[i++]);
	}
	free((*tmp)->redirections);
	if ((*tmp)->prev)
	{
		(*tmp)->prev->next = (*next)->next;
		(*tmp)->prev->in = STDIN;
	}
	else
	{
		(*next)->prev = NULL;
		data->cmd_table = (*next);	
		(*next)->in = STDIN;
	}
	free((*tmp));
}

void	cmd_table_no_vars(t_minishell *data)
{
	t_cmd_table	*tmp;
	t_cmd_table	*next;

	tmp = data->cmd_table;
	while (tmp)
	{
		next = tmp->next;
		if (!check_variable(tmp))
			clean_table_var(&tmp, &next, data);
		tmp = next;
	}
}
void	add_local_variables(char **new_env, char **new_export, t_minishell *data, t_cmd_table *tmp)
{
	if (tmp->n_args)
		variable_with_quotes(&tmp, data);
	if (!variable_in_env_char(tmp->cmd, data->env_vars))
	{
		new_env = mod_var(data->env_vars, data, tmp->cmd);
		free_arr(data->env_vars);
		data->env_vars = new_env;
	}
	if (!variable_in_env_char(tmp->cmd, data->export_vars))
	{
		new_export = mod_var_export(data->export_vars, data, tmp->cmd);
		free_arr(data->export_vars);
		data->export_vars = new_export;
	}
	local_variables_aux(data, tmp);
}

int	break_variables_loop(t_minishell *data)
{
	clean_local_vars(&data->local_vars);
	cmd_table_no_vars(data);
	return (1);
}

void	local_variables(t_minishell *data)
{
	t_cmd_table	*tmp;
	char		**new_env;
	char		**new_export;
	char		*expanded;
	bool		var;

	tmp = data->cmd_table;
	new_env = NULL;
	new_export = NULL;
	var = false;
	while (tmp)
	{
		expanded = expand(tmp->cmd, 0, data);
		free(tmp->cmd);
		tmp->cmd = expanded;
		if (!check_variable(tmp))
		{
			add_local_variables(new_env, new_export, data, tmp);
			var = true;
		}
		else if (check_variable(tmp) && var == true)
			if (break_variables_loop(data))
				return ;
		tmp = tmp->next;
	}
}
