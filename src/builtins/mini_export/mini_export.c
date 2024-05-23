/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:59:49 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/23 10:42:16 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_table(t_cmd_table *cmd_table) // borrar
{
	int i = 0; 

	while (cmd_table) 
	{
		printf("CMD:%s\n",cmd_table->cmd);
		if (cmd_table->args)
		{
			while (cmd_table->args[i])
			{
				printf("ARG%d:%s\n", i, cmd_table->args[i]);
				i++;
			}

		}
		if (cmd_table->in)
			printf("IN:%d\n", cmd_table->in);
		printf("OUT:%d\n", cmd_table->out);
		printf("ERR:%d\n", cmd_table->err);
		i = 0;
		while (i < cmd_table->n_redirections)
		{
			printf("redir number: %d type: %d value: %s\n", i, cmd_table->redirections[i]->type, cmd_table->redirections[i]->value);
			i++;
		}
		printf("nbr redir: %d\n", cmd_table->n_redirections);
		// if (cmd_table->new_cmd)
		// 	printf("new cdm TRUE\n");
		// else
		// 	printf("new cdm FALSE\n");
		cmd_table = cmd_table->next;
		printf("\n");
	}
}

char	*get_new_var(char *variable, t_variable *local_vars, t_minishell *data)
{
	t_variable	*it;
	int			name_size;
	char		*new_var;
	int			i;
	int			j;

	it = local_vars;
	name_size = get_var_size(variable, true);
	while (ft_strncmp(it->name, variable, name_size))
		it = it->next;
	new_var = (char *)malloc(ft_strlen(it->name) + ft_strlen(it->value) + 2);
	if (!new_var)
		error(data, "Memory problems while creating a new variable");
	i = 0;
	j = 0;
	while (it->name[i])
		new_var[j++] = it->name[i++];
	i = 0;
	new_var[j++] = '=';
	while (it->value[i])
		new_var[j++] = it->value[i++];
	new_var[j] = '\0';
	return (new_var);
}

void	declaration(t_minishell *data, int i, int laps, char **new_env)
{
	char	**new_export;
	
	if (!variable_in_env_char(data->cmd_table->args[i], data->env_vars))
	{
		new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
		new_export = mod_var(data->export_vars, data, data->cmd_table->args[i]);
	}
	else
	{
		if (laps < 0)
			create_new_variable(data->cmd_table->args[i],
				&(data->local_vars), data);
		else
			change_var_value(data->cmd_table->args[i],
				&(data->local_vars), laps, data);
		if (laps >= 0 && !variable_in_env(get_var_to_mod(data->local_vars,
					laps), data->env_vars))
			{
				new_env = mod_var(data->env_vars, data, data->cmd_table->args[i]);
				new_export = mod_var_export(data->export_vars, data, data->cmd_table->args[i]);
			}
		else
		{
			new_env = add_variable(data->cmd_table->args[i],
					data->env_vars, data);
			if (!variable_in_env_char(data->cmd_table->args[i], data->export_vars))
			{
				new_export = mod_var_export(data->export_vars, data, data->cmd_table->args[i]); 
			}
			else
				new_export = add_variable(data->cmd_table->args[i], data->export_vars, data);
		}
	}
	(free_arr(data->export_vars), free_arr(data->env_vars));
	data->env_vars = new_env;
	data->export_vars = new_export;
}

char	**add_var_no_value(char *variable, char **exp, t_minishell *data)
{
	int		nbr_export;
	int		i_j[2];
	char	**new_export;

	i_j[0] = 0;
	i_j[1] = 0;
	nbr_export = get_nbr_env(exp);
	new_export = (char **)malloc(sizeof(char *) * (nbr_export + 2));
	if (!new_export)
		error(data, "Memory problems in mini_export");
	while (i_j[0] < nbr_export && exp[i_j[0]])
	{
		new_export[i_j[1]] = ft_strdup(exp[i_j[0]]);
		if (!new_export[i_j[1]])
			(free_arr(new_export), error(data, "Memory problems in mini_export"));
		i_j[0]++;
		i_j[1]++;
	}
	new_export[i_j[1]] = ft_strdup(variable);
	if (!new_export[i_j[1]])
		(free_arr(new_export), error(data, "Memory problems in mini_export"));
	i_j[1]++;
	new_export[i_j[1]] = NULL;
	return (new_export);
}

void	no_declaration_aux(t_minishell *data, int i, char **new_export)
{
	if (variable_in_env_char(data->cmd_table->args[i], data->export_vars))
	{
		new_export = add_var_no_value(data->cmd_table->args[i], data->export_vars, data);
		free_arr(data->export_vars);
		data->export_vars = new_export;	
	}
}

void	no_declaration(t_minishell *data, int i, char *new_var, char **new_env)
{
	char	**new_export;
	
	new_export = NULL;
	if (check_new_var(data->cmd_table->args[i], data->local_vars) >= 0)
	{
		if (variable_in_env_char(data->cmd_table->args[i], data->export_vars))
		{
			new_var = get_new_var(data->cmd_table->args[i], data->local_vars, data);
			new_export = add_variable(new_var, data->export_vars, data); 
			(free_arr(data->export_vars), free(new_var));
			data->export_vars = new_export;
		}
		if (variable_in_env_char(data->cmd_table->args[i], data->env_vars))
		{
			new_var = get_new_var(data->cmd_table->args[i], data->local_vars, data);
			new_env = add_variable(new_var, data->env_vars, data);
			(free_arr(data->env_vars), free(new_var));
			data->env_vars = new_env;
		}
	}
	else
		no_declaration_aux(data, i, new_export);
}

void	reset_export(t_minishell *data, int i, int *declaration, int *laps)
{
	char *expanded;

	expanded = expand(data->cmd_table->args[i], 0, data);
	free(data->cmd_table->args[i]);
	data->cmd_table->args[i] = expanded;
	*declaration = check_if_declaration(data->cmd_table->args[i]);
	*laps = check_new_var(data->cmd_table->args[i], data->local_vars);
}

int		get_new_args_nbr(char **args)
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

char	*take_out_quotes(char **args, int i)
{
	char *new_arg;
	int	j;
	int	w;

	j = 0;
	w = 0;
	new_arg = (char *)malloc(ft_strlen(args[i]) + 1);
	//proteger
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
	int		i;
	int		j;
	int		w;
	char	*tmp;
	char	*middle;
	char	**new_args;

	i = 0;
	w = 0;
	j = 1;
	tmp = NULL;
	new_args = (char **)malloc(sizeof(char *) * (data->cmd_table->n_args + 1));
	// proteger
 	while (i < data->cmd_table->n_args)
	{
		if (!is_pos_variable(data->cmd_table->args[i]) && !get_var_size(data->cmd_table->args[i], false))
		{
			while (j < data->cmd_table->n_args && is_pos_variable(data->cmd_table->args[j]))
			{
				if (data->cmd_table->args[j] && !is_quoted_var(data->cmd_table->args[j]))
				{
					if (!tmp)
					{
						tmp = (char *)malloc(ft_strlen(data->cmd_table->args[i]) + ft_strlen(data->cmd_table->args[j]) + 1);
						//proteger
						ft_strlcpy(tmp, data->cmd_table->args[i], ft_strlen(data->cmd_table->args[i]) + 1);
					}
					data->cmd_table->args[j]++;
					if (!quote_in_middle(data->cmd_table->args[j], (*(data->cmd_table->args[j] - 1))))
					{
						middle = take_out_quotes(data->cmd_table->args, j);
						ft_strlcat(tmp, middle, (ft_strlen(tmp) + ft_strlen(data->cmd_table->args[j]) + 1));
						free(middle);
					}
					else
						ft_strlcat(tmp, data->cmd_table->args[j], (ft_strlen(tmp) + ft_strlen(data->cmd_table->args[j]))); 
					data->cmd_table->args[j]--;
					
				}
				j++;
			}
			if (j == i + 1)
				new_args[w] = ft_strdup(data->cmd_table->args[i]);
			else
				new_args[w] = ft_strdup(tmp);
			// proteger
			w++;
			free(tmp);
			tmp = NULL;
		}
		else if (is_quoted_var(data->cmd_table->args[i]))
		{
			new_args[w] = ft_strdup(data->cmd_table->args[i]);
			w++;
		}
		i++;
		j = i + 1;
	}
	new_args[w] = NULL;
	free_arr(data->cmd_table->args);
	data->cmd_table->args = new_args;
	data->cmd_table->n_args = get_new_args_nbr(data->cmd_table->args);
}

int	mini_export(t_minishell *data)
{
	int		i;
	int		dec;
	int		laps;
	char	**new_env;
	char	*new_var;

	i = 0;
	if (data->cmd_table->n_args == 0)
		env_order(data);
	new_env = NULL;
	new_var = NULL;
	check_possible_quotes(data);
	// print_cmd_table(data->cmd_table);
	// exit(0);
	while (i < data->cmd_table->n_args)
	{
		reset_export(data, i, &dec, &laps);
		if (!dec)
			declaration(data, i, laps, new_env);
		if (dec)
			no_declaration(data, i, new_var, new_env);
		i++;
	}
	return (0);
}
