/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:30:07 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/07 19:00:37 by cmunoz-g         ###   ########.fr       */
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


int		is_pos_variable(char *value)
{
	int		i;

	i = 0;
	if (value[i++] == '=')
		return (1);
	while (value[i])
	{
		if (value[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

void	check_local_var(t_token **token_list) 
{
	t_token	*it_variable;

	it_variable = *(token_list);
	while (it_variable)
	{
		if (!is_pos_variable(it_variable->value))
			it_variable->variable = true;
		if (!it_variable->prev && it_variable->variable)
			it_variable->type = CMD;
		else if (it_variable->prev && it_variable->variable)
		{
			if (it_variable->prev->variable)
				it_variable->type = CMD;
		}
		it_variable = it_variable->next;
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
	check_local_var(list); // checkea si deberia cambiar las local var por CMDS
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