#include "minishell.h"

long long	ft_atoll(char *str)
{
	long long	result;
	long long	negative;
	int			i;

	i = 0;
	result = 0;
	negative = 1;
	if (str[i] == '-')
	{
		i++;
		negative = -1;
	}
	while (str[i])
	{
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return (result * negative);
}

int	check_if_too_big(char *arg)
{
	int	chars;

	if (!arg || !arg[0])
		return (0);
	chars = ft_strlen(arg);
	if (chars > 20)
		return (1);
	else if (arg[0] == '-' && chars == 20 && ft_strncmp("-9223372036854775808", arg, 21) < 0)
		return (1);
	else if (arg[0] != '-' && chars == 19 && ft_strncmp("9223372036854775807", arg, 21) < 0)
		return (1);
	return (0);
}

int	check_if_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] < 48 || arg[i] > 57)
			return (1);
		i++;
	}
	return (0);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	mini_exit(t_minishell *data) 
{
	long long exit_code;

	exit_code = 0;
	if (data->cmd_table->args)
	{
		if (data->cmd_table->n_args > 1)
			(printf("minishell: exit: too many arguments\n"), error_builtins(data, 1));
		if (data->cmd_table->args[0])
		{
			if (check_if_number(data->cmd_table->args[0]) || check_if_too_big(data->cmd_table->args[0]))
			{
				printf("minishell: exit: %s: numeric argument required\n", data->cmd_table->args[0]);
				error_builtins(data, 255);
			}
			exit_code = ft_atoll(data->cmd_table->args[0]);
		}
	}
	exit_code %= 256;
	if (exit_code < 0)
		exit_code += 256;
	if (data->line)
		free(data->line);
	if (data->token_list)
		free(data->token_list);
	if (data->local_vars)
		free(data->local_vars);
	if (data->env_vars)
		free_arr(data->env_vars);
	if (data->pwd)
		free(data->pwd);
	if (data->old_pwd)
		free(data->old_pwd);
	clean_cmd_table_list(&(data->cmd_table));
	free(data);
	(printf("exit\n"), exit(exit_code));
}

// Codigo original, lo dejo por aqui. 

/*
void	mini_exit(t_minishell *data, t_cmd_table *cmd) 
{
	long long exit_code;

	exit_code = 0;
	//clean_cmd_table_list(&(data->cmd_table)); // esto esta bien aqui? comprobar que no haya doble frees
	printf("%s\n",data->cmd_table->cmd);
	exit(0);
	if (cmd->args)
	{
		if (cmd->n_args > 1)
			(printf("minishell: exit: too many arguments\n"), error_builtins(data, 1));
		if (cmd->args[0])
		{
			if (check_if_number(cmd->args[0]) || check_if_too_big(cmd->args[0]))
			{
				printf("minishell: exit: %s: numeric argument required\n", cmd->args[0]);
				error_builtins(data, 255);
			}
			exit_code = ft_atoll(cmd->args[0]);
		}
	}
	exit_code %= 256;
	if (exit_code < 0)
		exit_code += 256;
	printf("exit\n");
	exit(exit_code);
}*/