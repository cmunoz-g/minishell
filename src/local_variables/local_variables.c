/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:16:35 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/10 11:50:09 by camunozg         ###   ########.fr       */
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

int	get_new_var_space(t_cmd_table *tmp)
{
	int		res;
	int		i;
	int		j;
	bool	in_quotes;
	char	quote;

	j = 0;
	i = 0;
	res = 0;
	quote = '\0';
	in_quotes = false;
	while (tmp->args[i])
	{
		while (tmp->args[i][j])
		{
			if (tmp->args[i][j] == quote && in_quotes == true)
				in_quotes = false;
			if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
			{
				in_quotes = true;
				quote = tmp->args[i][j];
			}
			if (in_quotes == true || (tmp->args[i][j] != '\'' && tmp->args[i][j] != '\"'))
				res++;
			j++;
		}
		i++;
		j = 0;
	}
	return (res);
}

void	variable_with_quotes(t_cmd_table **tmp)
{
	int		i;
	int		j;
	int		w;
	int		new_space;
	char	*new_cmd;
	bool	quote;
	char	quote_type;

	quote = false;
	quote_type = '\0';
	i = 0;
	j = 0;
	w = ft_strlen((*tmp)->cmd);
	new_space = get_new_var_space((*tmp)); 
	new_cmd = (char *)malloc(w + new_space + 1);
	//if (!new_cmd)
	// proteger
	ft_strlcpy(new_cmd, (*tmp)->cmd, (size_t)(w + 1));
	while (i < (*tmp)->n_args)
	{
		while ((*tmp)->args[i][j])
		{
			if ((*tmp)->args[i][j] == quote_type && quote == true)
				quote = false;
			if (quote == false && ((*tmp)->args[i][j] == '\'' || (*tmp)->args[i][j] == '\"'))
			{
				quote = true;
				quote_type = (*tmp)->args[i][j];
			}
			if ((quote == true && quote_type != (*tmp)->args[i][j]) || ((*tmp)->args[i][j] != '\'' && (*tmp)->args[i][j] != '\"'))
			{
				new_cmd[w] = (*tmp)->args[i][j];
				w++;
			}
			j++;
		}
		j = 0;
		i++;
	}
	new_cmd[w] = '\0';
	free_arr((*tmp)->args);
	(*tmp)->args = NULL;
	free((*tmp)->cmd);
	(*tmp)->cmd = new_cmd;
	(*tmp)->n_args = 0;
}

void	cmd_table_no_vars(t_minishell *data)
{
	t_cmd_table	*tmp;
	t_cmd_table	*next;
	int			i;

	i = 0;
	tmp = data->cmd_table;
	while (tmp)
	{
		next = tmp->next;
		if (!check_variable(tmp))
		{
			free(tmp->cmd);
			free_arr(tmp->args);
			while (i < tmp->n_redirections)
			{
				free(tmp->redirections[i]->value);
				free(tmp->redirections[i++]);
			}
			free(tmp->redirections);
			if (tmp->prev)
			{
				tmp->prev->next = next->next;
				tmp->prev->in = STDIN;
			}
			else
			{
				next->prev = NULL;
				data->cmd_table = next;	
				next->in = STDIN;
			}
			free(tmp);
		}
		tmp = next;
	}
}

void	local_variables(t_minishell *data) // DEBEN ACTUALIZAR EXPORT TAMBIEN
{
	t_cmd_table	*tmp;
	char		**new_env;
	char		**new_export;
	bool		var;

	tmp = data->cmd_table;
	new_env = NULL;
	new_export = NULL;
	var = false;
	while (tmp)
	{
		if (!check_variable(tmp))
		{
			if (tmp->n_args)
				variable_with_quotes(&tmp);
			if (!variable_in_env_char(tmp->cmd, data->env_vars))
			{
				new_env = mod_var(data->env_vars, data, tmp->cmd);
				free_arr(data->env_vars);
				data->env_vars = new_env;
			}
			if (!variable_in_env_char(tmp->cmd, data->export_vars))
			{
				new_export = mod_var_export(data->env_vars, data, tmp->cmd);
				free_arr(data->export_vars);
				data->export_vars = new_export;
			}
			else
				local_variables_aux(data, tmp);
			var = true;
		}
		else if (check_variable(tmp) && var == true)
		{
			clean_local_vars(&data->local_vars);
			cmd_table_no_vars(data);
			return ;
		}
		tmp = tmp->next;
	}
}
