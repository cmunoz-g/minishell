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

int	mini_exit(t_minishell *data) 
{
	long long	exit_code;

	exit_code = 0;
	if (data->cmd_table->args)
	{
		if (data->cmd_table->n_args > 1)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		if (data->cmd_table->args[0])
		{
			if (check_if_number(data->cmd_table->args[0])
				|| check_if_too_big(data->cmd_table->args[0]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(data->cmd_table->args[0], 2);
				ft_putendl_fd(": numeric argument required", 2);
				return (255);
			}
			exit_code = ft_atoll(data->cmd_table->args[0]);
		}
	}
	exit_code %= 256;
	if (exit_code < 0)
		exit_code += 256;
	clean_data(&data);
	(ft_putendl_fd("exit", 1), exit(exit_code));
}
