/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:16:35 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 19:36:24 by cmunoz-g         ###   ########.fr       */
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

void	variable_with_quotes(t_cmd_table **tmp)
{
	int		i;
	int		j;
	int		w;
	int		new_space;
	char	*new_cmd;

	i = 0;
	j = 0;
	w = ft_strlen(tmp->cmd);
	new_space = get_new_var_space(tmp); // calcular cuanto espacio necesitamos en cmd, sin contar comillas
	new_cmd = (char *)malloc(ft_strlen(tmp->cmd) + new_space + 1);
	if (!new_cmd)
	// proteger
	ft_strlcpy(new_cmd, tmp->cmd, w);
	while (i < (*tmp)->n_args)
	{
		while (args[i][j])
		{
			if (args[i][j] == '\'' || args[i][j] == "\"")
				j++;
			else
				new_cmd[w] = args[i][j];
			j++;
			w++;
		}
		j = 0;
		i++;
	}
	new_cmd[w] = '\0';
}

void	local_variables(t_minishell *data) // el tema de las local variables se que ha quedado aqui, nose porque cuando mandas a=1 b=2 no esta reconociendo el primer comando, mirar como se esta creando la cmd table
{
	t_cmd_table	*tmp;
	char		**new_env;

	tmp = data->cmd_table;
	new_env = NULL;
	print_cmd_table(tmp);
	exit(0);
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
			else
				local_variables_aux(data, tmp);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}

/*minishell> a="112"'awsd'
CMD:a=
ARG0:"112"
ARG1:'awsd'*/
