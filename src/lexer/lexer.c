/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:30:07 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/08 13:51:36 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* El nombre de variable token_list ha sido modificado por list*/

// PRUEBAS: a=1 b=2 crea solo b=2

// a=hola quetal 

// cmunoz-g@c1r3s2:~/Documents/minishell$ a=hola quetal
// equetal: command not found
// cmunoz-g@c1r3s2:~/Documents/minishell$ echo $a

// cmunoz-g@c1r3s2:~/Documents/minishell$ hola="hola"
// cmunoz-g@c1r3s2:~/Documents/minishell$ echo $hola
// hola

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

int	is_there_space(t_token *it_variable, char *line)
{
	char	*str;
	char	*tmp;
	int		laps;

	tmp = line;
	laps = check_if_repeated_value(it_variable);
	if (laps)
	{
		while (laps != 0)
		{
			str = ft_strnstr(tmp, it_variable->value, ft_strlen(tmp)) + ft_strlen(it_variable->value) + 1;
			laps--;
		}
		str = ft_strnstr(str, it_variable->value, ft_strlen(tmp) + 1);
		str--;
		if (*str && *str == ' ')
			return (0);
		else
			return (1);
	}
	str = ft_strnstr(tmp, it_variable->value, ft_strlen(tmp));
	str--;
	if (*str && *str == ' ')
		return (0);
	return (1);
}

void	check_local_var(t_token **token_list, char *line) 
{
	t_token	*it_variable;

	it_variable = *(token_list);
	while (it_variable)
	{
		if (!is_pos_variable(it_variable->value))
			it_variable->variable = true;
		if (!it_variable->prev && it_variable->variable == true)
			it_variable->type = CMD;
		else if (it_variable->prev && it_variable->variable == true)
		{
			if (it_variable->prev->variable || it_variable->prev->type != CMD)
				it_variable->type = CMD;
		}
		else if (it_variable->prev && it_variable->variable == false)
		{
			if (it_variable->prev->variable == true && is_quoted_var(it_variable->value))
				it_variable->type = CMD;
			else if ((!is_there_space(it_variable, line)) && !is_quoted_var(it_variable->value)) 
				it_variable->type = CMD;
		}
		it_variable = it_variable->next;
	}
}

void	print_tokens(t_token *token_list) // borrar
{
	while (token_list)
	{
	 	printf("value:%s type:%d\n",token_list->value, token_list->type);
	 	token_list = token_list->next;
	}
}

void	lexer(char *line, t_token **list)
{
	int		i;
	int		st;
	char	qt_type;
	bool	qt;

	init_lexer(&qt, &i, &st);
	while (line[++i])
	{
		if (qt && (line[i] == qt_type && (i == 0 || line[i - 1] != '\\')))
			qt = false;
		else if (!qt)
		{
			if (line[i] == '"' || line[i] == '\'')
				(set_qt(&qt, &qt_type, line, i), lexer_qt(list, line, &st, i));
			else if (ft_isspace(line[i]) || line[i] == '<' || line[i] == '>')
				lexer_new_token(list, line, &st, &i);
			else if (line[i] == ';' || line[i] == '|')
				lexer_new_token_aux(list, line, &st, &i);
		}
	}
	if (i > (st))
		add_token(list, line, st, i);
	check_redirections(list);
	check_local_var(list, line); // checkea si deberia cambiar las local var por CMDS
}

void	lexer_qt(t_token **token_list, char *cmd_line, int *start, int i)
{
	if (i > (*start))
	{
		add_token(token_list, cmd_line, (*start), i);
		(*start) = i;
	}
}

void	lexer_new_token_aux(t_token **list, char *line, int *start, int *i)
{
	if ((*i) > (*start))
		add_token(list, line, (*start), (*i));
	if (line[*i] == '|')
	{
		if (line[*i + 1] == '|')
		{
			add_token(list, line, (*i), (*i) + 2);
			(*i)++;
		}
		else
			add_token(list, line, (*i), (*i) + 1);
		(*start) = (*i) + 1;
	}
	else if (line[*i] == ';')
	{
		add_token(list, line, (*i), (*i) + 1);
		(*start) = (*i) + 1;
	}
}

void	lexer_new_token(t_token **list, char *line, int *start, int *i)
{
	if (ft_isspace(line[*i]))
	{
		if (*i > *start)
			add_token(list, line, *start, *i);
		*start = *i + 1;
	}
	else
	{
		if ((*i) > (*start))
			add_token(list, line, (*start), (*i));
		if (line[(*i)] == '>' && line[(*i) + 1] == '>')
		{
			add_token(list, line, (*i), (*i) + 2);
			(*i)++;
		}
		else if (line[(*i)] == '<' && line[(*i) + 1] == '<')
		{
			add_token(list, line, (*i), (*i) + 2);
			(*i)++;
		}
		else
			add_token(list, line, (*i), (*i) + 1);
		(*start) = (*i) + 1;
	}
}

void	check_redirections(t_token **token_list)
{
	t_token	*it_filename;

	it_filename = *(token_list);
	while (it_filename)
	{
		if ((it_filename->next && !ft_strcmp(it_filename->value, "2")
				&& (it_filename->next->type == HEREDOC
					|| it_filename->next->type == TRUNC
					|| it_filename->next->type == APPEND
					|| it_filename->next->type == INPUT)))
			it_filename->type = STDERR;
		if ((it_filename->next && (!ft_strcmp(it_filename->value, "1"))
				&& (it_filename->next->type == HEREDOC
					|| it_filename->next->type == TRUNC
					|| it_filename->next->type == APPEND
					|| it_filename->next->type == INPUT)))
			it_filename->type = STDOUT;
		it_filename = it_filename->next;
	}
}