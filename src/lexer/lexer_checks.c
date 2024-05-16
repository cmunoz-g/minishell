#include "minishell.h"

int	is_there_space(t_token *it_variable, char *line)
{
	char	*str;
	char	*tmp;
	int		laps;

	tmp = line;
	laps = check_if_repeated_value(it_variable);
	if (is_quoted_var(it_variable->value))
		return (1);
	if (laps)
	{
		while (laps != 0)
		{
			str = ft_strnstr(tmp, it_variable->value, ft_strlen(tmp))
				+ ft_strlen(it_variable->value) + 1;
			laps--;
		}
		str = ft_strnstr(str, it_variable->value, ft_strlen(tmp) + 1) - 1;
		if (*str && *str == ' ')
			return (0);
		return (1);
	}
	str = ft_strnstr(tmp, it_variable->value, ft_strlen(tmp)) - 1;
	if (*str && *str == ' ')
		return (0);
	return (1);
}

int	check_if_variable(t_token *it_variable)
{
	t_token *tmp;

	tmp = it_variable;
	while (tmp->prev)
	{
		if (tmp->prev->type == CMD && tmp->prev->variable == true)
			return (0);
		else if (tmp->type == PIPE)
			return (1);
		tmp = tmp->prev;
	}
	return (1);
}

int	check_first_cmd(t_token *it_variable)
{
	t_token	*tmp;

	tmp = it_variable;
	while (tmp->prev)
		tmp = tmp->prev;
	if (!ft_strncmp("export", tmp->value, 6))
		return (0);
	else
		return (1);
}

int	check_if_pipe(t_token *it_variable)
{
	t_token *tmp;

	tmp = it_variable;
	while (tmp->prev)
	{
		if (tmp->type == PIPE)
			return (1);
		tmp = tmp->prev;
	}
	return (0);
}

int	check_if_repeated_value(t_token *it_variable)
{
	t_token *tmp;
	int		i;
	
	i = 0;
	while (it_variable)
	{
		tmp = it_variable->prev;
		while (tmp)
		{
			if (!ft_strcmp(tmp->value, it_variable->value))
				i++;
			tmp = tmp->prev;
		}
		it_variable = it_variable->prev;
	}
	return (i);
}